#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

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
        double step(double dt = 0) {
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
        void getDeviceState() {
            _x = pdev->x();
            _v = pdev->v();
        }
        //розрахувати та встановити керуючий сигнал F
        double setDeviceControl() {
            _F = (_desired_x - _x)*Kp + _v*Kd;
            pdev->setControl(_F);
            return _F;        
        }
        //встанови
        void setDesiredState(double desired_x) {  _desired_x = desired_x; }
        //крок моделювання для системи управління. 
        //desired_x - задане значення x
        //dt - крок інтегрування 
        double step(double dt) {
            getDeviceState(); //отримати поточний стан пристрою
            setDeviceControl(); //розрахувати та встановити керуючу дію
            pdev->step(dt); //крок моделювання для керованого пристрою
        }
        CDevice& device() { return *pdev; } //посилання на керований об'єкт
};

int main(int argc, char const *argv[])
{
    int choice;
    const char* menu = 
    "1. Test device\n"
    "2. Test control system\n"
    "0. Exit\n";
    //об'єкт керованого пристрою
    CDevice* dev;
    //система управління
    CControl* ctrl;
    do {
        cout << menu;
        do {
            if (!cin) {cin.clear(); cin.ignore(255);}
            cout << ">";
            cin >> choice;
        } while (!cin);
        double dt;
        double F;
        switch (choice) {
            case 1:
                cout << "Enter dt: ";
                cin >> dt;
                cout << "Enter F: ";
                cin >> F;
                //об'єкт керованого пристрою
                dev = new CDevice(1, 1.5, 0, -2); //m, R, x0, v0
                dev->setControl(F); //задати керуючу дію
                dev->step(0); //розрахувати стан на t=0  (прискорення)
                cout << "Test with dt=" << fixed << setprecision(3) << dt << ", F=" << F 
                << ", x0=" << dev->x() << ", v0=" << dev->v() << endl;
                cout << "t \tx(t) \tdx/dt \tv(t) \tdv/dt \ta(t) \tR*v(t)+m*a(t)" << endl;
                double x0; double v0; //змінні для розрахунку діференціалів
                for (int i = 0; i < 20; i++) {
                    cout << fixed << setw(5) << setprecision(3) << i*dt << '\t' << dev->x() << '\t'; 
                    //розрахунок dx/dt. Має приблизно дорівнювати v(t)
                    if (i > 0) {
                        cout << (dev->x()-x0)/dt << '\t';
                    } else
                        cout << "-\t";
                    x0 = dev->x();
                    cout << dev->v() << '\t';
                    //розрахунок dv/dt. Має приблизно дорівнювати a(t)
                    if (i > 0) {
                        cout << (dev->v()-v0)/dt << '\t';
                    } else
                        cout << "-\t";
                    v0 = dev->v();
                    cout << dev->a() << '\t'
                    //розрахунок сумарних сил має приблизно дорівнювати 0
                    << setprecision(8) << F - dev->R()*dev->v() - dev->m()*dev->a() << endl;
                    dev->step(dt); //наступний крок моделювання приладу
                }
                delete dev;
                break;
            case 2:
                //об'єкт керованого пристрою
                dev = new CDevice(1, 1.5, 0, 0); //m, R, x0, v0
                //система управління
                ctrl = new CControl(*dev, 1, 1.5, 0, 0); //controlled device,  m, R, x0, v0
                ctrl->setPID(1, 0, 0); //kp, ki, kd
                cout << "Initial device state: x0=" << fixed << setprecision(3) 
                << ", x0=" << dev->x() << ", v0=" << dev->v() << endl 
                << "Desired state: x=";
                dt = 0.1; //крок моделювання
                ctrl->setDesiredState(1); //задане значення x
                cout << "t \tx(t) \tF(t) " << endl;
                for (int i = 0; i < 100; i++) {
                    cout << fixed << setw(5) << setprecision(3) << i*dt << '\t' << dev->x() << '\t'
                    << dev->F() << endl;
                    ctrl->step(dt); //наступний крок моделювання приладу
                }                
                delete ctrl;
                delete dev;
                break;
            default:
                break;
        }
    } while (choice);
    
    return 0;
}
