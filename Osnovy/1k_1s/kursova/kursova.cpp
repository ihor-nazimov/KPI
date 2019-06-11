#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

//клас імітує поточний стан керованого пристрою на час t
class CDevice {
    protected:
        double _m;  //маса
        double _R;  //коефіцієнт тертя
        double _F;  //сила. Керуючий параметр
        double _t; //поточний час
        double _x; //поточна позиція
        double _v; //поточна швидкість
        double _a; //поточне прискорення

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
            control(0, 0);
        };
        ~CDevice(){};
        //змінити стан об'єкта на dt секунд під впливом сили force
        //повертає положення об'єкта на час t0+dt
        double control(double force = 0, double dt = 0.1) {
            if (dt < 0) throw;
            _F = force;
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
};

//клас системи управління. Наслідувані поля містять оцінки параметрів керованого пристрою CDevice
class CControl: public CDevice {
        CDevice* pdev; //вказівник на керований пристрій
    public:
        CControl(CDevice& device, double m = 1, double R = 1, double x0 = 0, double v0 = 0): CDevice(m,R, x0,v0) {
            pdev = &device;
        }

        CDevice& device() { return *pdev; } //посилання на керований об'єкт
};


int main(int argc, char const *argv[])
{
    int choice;
    const char* menu = 
    "1. Test dev\n"
    "0. Exit\n";
    //об'єкт керованого пристрою
    CDevice dev(1, 1, 0, 1); //m, R, x0, v0

    do {
        cout << menu;
        do {
            if (!cin) {cin.clear(); cin.ignore(255);}
            cout << ">";
            cin >> choice;
        } while (!cin);
        switch (choice) {
            case 1:
                cout << "Enter dt: ";
                double dt;
                cin >> dt;
                cout << "Enter F: ";
                double F;
                cin >> F;
                dev.control(F, 0); //розрахувати стан на t=0  (прискорення)
                cout << "Test with dt=" << fixed << setprecision(3) << dt << ", F=" << F 
                << ", x0=" << dev.x() << ", v0=" << dev.v() << endl;
                cout << "t\t x(t)\t dx/dt\t v(t)\t dv/dt\t a(t)\t R*v(t)+m*a(t)" << endl;
                double x0; double v0; //змінні для розрахунку діференціалів
                for (int i = 0; i < 20; i++) {
                    cout << fixed << setw(5) << setprecision(3) << i*dt << '\t' << dev.x() << '\t'; 
                    //розрахунок dx/dt. Має приблизно дорівнювати v(t)
                    if (i > 0) {
                        cout << (dev.x()-x0)/dt << '\t';
                    } else
                        cout << "-\t";
                    x0 = dev.x();
                    cout << dev.v() << '\t';
                    //розрахунок dv/dt. Має приблизно дорівнювати a(t)
                    if (i > 0) {
                        cout << (dev.v()-v0)/dt << '\t';
                    } else
                        cout << "-\t";
                    v0 = dev.v();
                    cout << dev.a() << '\t'
                    //розрахунок сумарних сил має приблизно дорівнювати 0
                    << setprecision(8) << F - dev.R()*dev.v() - dev.m()*dev.a() << endl;
                    dev.control(F, dt); //наступний крок моделювання під дією сили F
                }

                break;
            default:
                break;
        }
    } while (choice);
    
    return 0;
}
