#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cstring>
#include <typeinfo>

using namespace std;

// static ofstream fout;

// class ostream_log: public ostream {
// protected:
//     const char* log = "log.csv";
// public:
//     ostream_log() {
//     }
//     ~ostream_log() { 
//     }
//     void open() {
//         fout.open(log, ios::out); 
//     }
//     void close() {fout.close(); }
    
//     friend ostream& operator<<(ostream& out, char* s) {
//         cout << s;
//         fout.write(s, strlen(s)+1);
//         return out;
//     }
// };

//клас імітує поточний стан керованого пристрою на час t
class CDevice {
    protected:
        double _m;  //маса
        double _R;  //коефіцієнт тертя
        double _F=0;  //сила. Керуючий параметр
        double _t=0; //поточний час
        double _x=0; //поточна позиція
        double _v=0; //поточна швидкість
        double _a=0; //поточне прискорення

        //прогнозовані значення на t+dt
        double predicted_x(double dt) {
            return (_v*_R - _F) * (1 - exp(-_R*dt/_m))*_m/_R/_R + _F*dt/_R + _x;        }
        double predicted_v(double dt) {
            return ( exp(-_R*dt/_m) * (_v*_R - _F) + _F ) / _R;        }
        double predicted_a(double dt) {
            return - exp(-_R*dt/_m) * (_v*_R - _F) / _m;        }
    public:
        //ініціалізація об'єкту
        CDevice(double m = 1, double R = 1, double x0 = 0, double v0 = 0){
            if ( m <= 0 || R <= 0) throw;
            this->_m = m;
            this->_R = R;
            _t = 0; _x = x0; _v = v0;
            step(0);
        };
        ~CDevice(){};
        void setControl(double force = 0) { _F = force; }
        //змінити стан об'єкта на dt секунд під впливом сили force
        //повертає положення об'єкта на час t0+dt
        virtual double step(double dt = 0) {
            if (dt < 0) throw;
            _t += dt;
            // double e = exp(-_R*dt/_m);
            // double v0R_F = _v*_R - _F;
            double x1 = predicted_x(dt);
            double v1 = predicted_v(dt);
            double a1 = predicted_a(dt);
            _a = a1; _v = v1;
            return _x = x1;
        }

        double t() { return _t;}
        double x() { return _x;}
        double v() { return _v;}
        double a() { return _a;}
        double R() { return _R;}
        double m() { return _m;}
        double F() { return _F;}
};

//клас системи управління. Наслідувані поля містять оцінки параметрів керованого пристрою CDevice
class CControl: public CDevice {
    protected:
        CDevice* pdev; //вказівник на керований пристрій
        double _desired_x; //задане значення x
        double Kp=1;
        double Kd=0;
        double Ki=0;
    public:
        //device - посилання на керований пристрій
        CControl(CDevice& device, double m = 1, double R = 1, double x0 = 0, double v0 = 0): CDevice(m,R, x0,v0) {
            pdev = &device; }
        void  setPID(double kp, double ki, double kd) { Kp = kp; Kd = kd; Ki = ki; }
        //вимір поточного стану пристрою
        virtual double getDeviceState() {
            _x = pdev->x();
            _v = pdev->v();
            return _x;
        }
        //розрахувати та встановити керуючий сигнал F
        virtual double setDeviceControl() {
            _F = (_desired_x - _x)*Kp + _v*Kd; //пропорц.-дифф. регулятор 
            pdev->setControl(_F);
            return _F;        
        }
        //встанови
        virtual void setDesiredState(double desired_x) {  _desired_x = desired_x; }
        //крок моделювання для системи управління. 
        //desired_x - задане значення x
        //dt - крок інтегрування 
        virtual double step(double dt) {
            if (dt < 0) throw;
            _t += dt;
            getDeviceState(); //отримати поточний стан пристрою
            setDeviceControl(); //розрахувати та встановити керуючу дію
            pdev->step(dt); //крок моделювання для керованого пристрою
            return _x;
        }
        CDevice& device() { return *pdev; } //посилання на керований об'єкт
};

//режими екстраполяції
enum CorrectionMode {NONE,           //відсутня
                    DEVICE_SPECIFIC, //згідно моделі пристрою. Врахована сила на час виміру
                    DEVICE_SPECIFIC_INT, //згідно моделі пристрою. Врахована зміна сила підчас інтервалу 
                    LINEAR}; //не реалізовано

class CDiscontinuousControl: public CControl {
    protected:
        double _measureInterval = 0;
        CorrectionMode _mode = NONE;
    public:
        //початок кроку екстраполяції
        double t0 = 0; 
        double x0=0;
        double v0=0;
        //останні виміри
        double t0_msrd = 0;
        double x0_msrd=0;
        double v0_msrd=0;


        CDiscontinuousControl(CDevice& device, double m = 1, double R = 1, double x0 = 0, double v0 = 0): 
            CControl(device, m,R, x0,v0) {}
        CorrectionMode setCorrectionMode(CorrectionMode mode) { _mode = mode; }
        double measureInterval(double mi=-1) { 
            if (mi >= 0) _measureInterval = mi; 
            return _measureInterval;
        }
        double getDeviceState() {
            //поточний час моделювання вже оновлений до _t
            if ( (_t - t0_msrd) >= _measureInterval ) {
                //виміряти стан пристрою, якщо минув інтервал вимірювання
                _x = pdev->x();
                //зберегти значення на час вимірувань
                t0_msrd = _t;
                x0_msrd = _x;
                v0_msrd = _v;
                switch (_mode){
                    case DEVICE_SPECIFIC: {
                        double e = exp(-_R*(_t-t0)/_m); //тимчасова змінна
                        //розрахувати скориговане значення v0 для наступного інтервалу екстраполяції
                        // Минуле вимірювання:  t0: _t0       x0:   x0      v0:   v0
                        // Поточне вимірювання: t1: _t        x1:   _x      v1:   v0 (нове значення) 
                        _v = v0 = _F/_R * ( e * (_R*(_x-x0)-_F*(_t-t0)) / (1-e)/_m/_F + 1 );
                        //зберегти виміряні значення
                        x0 = _x;
                        break;
                    }
                    case DEVICE_SPECIFIC_INT:
                        //розрахувати скориговане значення v0 для наступного інтервалу екстраполяції
                        // Минуле вимірювання:  t0: _t0       x0:   x0      v0:   v0
                        // Поточне вимірювання: t1: _t        x1:   _x      v1:   v0 (нове значення) 
                        //_v = v0 = _F/_R * ( e * (_R*(_x-x0)-_F*(_t-t0)) / (1-e)/_m/_F + 1 );
                        //зберегти виміряні значення
                        x0 = _x;
                        break;
                }
                //зберегти виміряні значення
                x0 = _x;
                v0 = _v;
                t0 = _t;
            }
            //екстраполяція 
            switch (_mode) {
                case DEVICE_SPECIFIC: {
                    double e = exp(-_R*(_t-t0)/_m); //тимчасова змінна
                    double v0r_f = v0*_R-_F;
                    //екстраполяція 
                    // Минуле вимірювання:  t0: _t0       x0:   x0      v0:   v0
                    // Екстраполяція:        t: _t-t0     x(t): _x      v(t): _v
                    _v = ( e * v0r_f + _F ) / _R;
                    _x = v0r_f * _m/_R/_R * (1-e) + _F*(_t-t0)/_R + x0;
                    _a = - e * v0r_f / _m;
                    break;
                }
                case DEVICE_SPECIFIC_INT: {
                    double e = exp(-_R*(_t-t0)/_m); //тимчасова змінна
                    double v0r_f = v0*_R-_F;
                    //екстраполяція 
                    // Минуле вимірювання:  t0: _t0       x0:   x0      v0:   v0
                    // Екстраполяція:        t: _t-t0     x(t): _x      v(t): _v
                    _v = ( e * v0r_f + _F ) / _R;
                    _x = v0r_f * _m/_R/_R * (1-e) + _F*(_t-t0)/_R + x0;
                    _a = - e * v0r_f / _m;
                    x0 = _x;
                    v0 = _v;
                    t0 = _t;

                    break;  
                }              
            }
            return _x;
        }
};

int main(int argc, char const *argv[])
{
    int choice;
    // const char* log_csv = "log.csv";
    // ofstream fo;
    // fo.open(log_csv, ios::out);
    ostream& fo = std::cout;
    const char* menu = 
    "1. Test device\n"
    "2. Continuous control system\n"
    "3. Discontinuous control system\n"
    "4. Corrected discontinuous control system\n"
    "0. Exit\n";
    //об'єкт керованого пристрою
    CDevice* dev;
    //система управління
    CControl* ctrl;
    CDiscontinuousControl* disctrl;
    double dt, ddt, F, kp, kd;
    do {
        cout << menu;
        do {
            if (!cin) {cin.clear(); cin.ignore(255);}
            cout << ">";
            cin >> choice;
        } while (!cin);
        switch (choice) {
            case 1: {
                cout << "Enter dt: ";
                cin >> dt;
                cout << "Enter F: ";
                cin >> F;
                //об'єкт керованого пристрою
                dev = new CDevice(1, 1.5, 0, -2); //m, R, x0, v0
                dev->setControl(F); //задати керуючу дію
                dev->step(0); //розрахувати стан на t=0  (прискорення)
                fo << "Test with dt=" << fixed << setprecision(3) << dt << ", F=" << F 
                << ", x0=" << dev->x() << ", v0=" << dev->v() << endl;
                fo << "t \tx(t) \tdx/dt \tv(t) \tdv/dt \ta(t) \tF-R*v(t)+m*a(t)" << endl;
                double x0; double v0; //змінні для розрахунку діференціалів
                for (int i = 0; i < 20; i++) {
                    fo << fixed << setw(5) << setprecision(3) << i*dt << '\t' << dev->x() << '\t'; 
                    //розрахунок dx/dt. Має приблизно дорівнювати v(t)
                    if (i > 0) {
                        fo << (dev->x()-x0)/dt << '\t';
                    } else
                        fo << "-\t";
                    x0 = dev->x();
                    fo << dev->v() << '\t';
                    //розрахунок dv/dt. Має приблизно дорівнювати a(t)
                    if (i > 0) {
                        fo << (dev->v()-v0)/dt << '\t';
                    } else
                        fo << "-\t";
                    v0 = dev->v();
                    fo << dev->a() << '\t'
                    //розрахунок сумарних сил має приблизно дорівнювати 0
                    << setprecision(8) << F - dev->R()*dev->v() - dev->m()*dev->a() << endl;
                    dev->step(dt); //наступний крок моделювання приладу
                }
                delete dev;
                break;
            }
            case 2:{
                //об'єкт керованого пристрою
                dev = new CDevice(1, 1.5, 0, 0); //m, R, x0, v0
                //система управління
                ctrl = new CControl(*dev, 1, 1.5, 0, 0); //controlled device,  m, R, x0, v0
                cout << "Enter Kp: ";
                cin >> kp;
                cout << "Enter Kd: ";
                cin >> kd;
                // ctrl->setPID(18, -2.7, 0); //kp, ki, kd
                ctrl->setPID(kp, kd, 0); //kp, ki, kd
                ddt = 0.1; //крок моделювання
                dt = 0.2; //крок друку значень

                fo << "Initial device state: x0=" 
                << fixed << setprecision(3) << dev->x() << ", v0=" << dev->v() << endl 
                << "Desired state: x=1.000" << endl; 

                ctrl->setDesiredState(1); //задане значення x
                fo << "t \tx(t) \tF(t) " << endl;
                for (long i = 0; i < 100; i++) {
                    fo << fixed << setw(5) << setprecision(3) << i*ddt*round(dt / ddt) << '\t' << dev->x() << '\t'
                    << dev->F() << endl;
                    for (long j = 0; j < round(dt / ddt); j++)
                        ctrl->step(ddt); //наступний крок моделювання приладу
                }    
                delete ctrl;
                delete dev;
                break;
            }
            case 3: {
                //об'єкт керованого пристрою
                dev = new CDevice(1, 1.5, 0, 0); //m, R, x0, v0
                //система управління
                disctrl = new CDiscontinuousControl(*dev, 1, 1.5, 0, 0); //controlled device,  m, R, x0, v0
                disctrl->measureInterval(1.0); //інтервал оновлення вхідних параметрів
                disctrl->setCorrectionMode(NONE);
                cout << "Enter Kp: ";
                cin >> kp;
                cout << "Enter Kd: ";
                cin >> kd;
                disctrl->setPID(kp, kd, 0); //kp, ki, kd
                ddt = 0.1; //крок моделювання
                dt = 0.2; //крок друку значень

                fo << "Initial device state: x0=" 
                << fixed << setprecision(3) << dev->x() << ", v0=" << dev->v() << endl 
                << "Desired state: x=1.000" << endl; 
                disctrl->setDesiredState(1); //задане значення x
                fo << "t \tx(t) \tx_msrd \tF(t) " << endl;
                for (long i = 0; i < 100; i++) {
                    fo << fixed << setw(5) << setprecision(3) << i*ddt*round(dt / ddt) << '\t' 
                    << dev->x() << '\t'
                    << disctrl->x0_msrd << '\t'
                    << dev->F() << endl;
                    for (long j = 0; j < round(dt / ddt); j++)
                        disctrl->step(ddt); //наступний крок моделювання приладу
                }                
                delete disctrl;
                delete dev;
                break;
            }
            case 4: {
                //об'єкт керованого пристрою
                double m = 1;
                double R = 1.5;
                cout << "Enter Kp: ";
                cin >> kp;
                cout << "Enter Kd: ";
                cin >> kd;
                dev = new CDevice(m, R, 0, 0); //m, R, x0, v0
                double delta;
                cout << "Enter deviation of measured m (%): ";
                cin >> delta;
                m = m*(1+delta/100);                
                cout << "Enter deviation of measured R (%): ";
                cin >> delta;
                R = R*(1+delta/100);                
                //система управління
                disctrl = new CDiscontinuousControl(*dev, m, R, 0, 0); //controlled device,  m, R, x0, v0
                disctrl->measureInterval(1.0); //інтервал оновлення вхідних параметрів
                disctrl->setCorrectionMode(DEVICE_SPECIFIC_INT);
                disctrl->setPID(kp, kd, 0); //kp, ki, kd
                ddt = 0.1; //крок моделювання
                dt = 0.2; //крок друку значень
                // fo.open();
                fo << "Initial device state: x0=" 
                << fixed << setprecision(3) << dev->x() << ", v0=" << dev->v() << endl 
                << "Desired state: x=1.000" << endl; 
                disctrl->setDesiredState(1); //задане значення x
                fo << "t \tx(t) \tx_msrd \tx_extr \tv(t) \tv_extr \ta(t) \ta_extr \tF(t) " << endl;
                for (long i = 0; i < 100; i++) {
                    fo << fixed << setw(5) << setprecision(3) << i*ddt*round(dt / ddt) << '\t' 
                    << dev->x() << '\t'
                    << disctrl->x0_msrd << '\t'
                    << disctrl->x() << '\t'
                    << dev->v() << '\t'
                    << disctrl->v() << '\t'
                    << dev->a() << '\t'
                    << disctrl->a() << '\t'
                    << dev->F() << endl;
                    for (long j = 0; j < round(dt / ddt); j++)
                        disctrl->step(ddt); //наступний крок моделювання приладу
                }             
                delete disctrl;
                delete dev;
                break;
            }
            case 0:
                cout << "Bye!" << endl;
                if (typeid(fo) == typeid(ofstream)) dynamic_cast<ofstream&>(fo).close();
            default:
                cout << "Incorrect choice!" << endl;
                break;
        }
    } while (choice);
    
    return 0;
}
