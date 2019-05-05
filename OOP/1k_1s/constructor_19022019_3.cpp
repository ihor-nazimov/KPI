//ініціалізація
#include <stdio.h>

class Point {
    private:
        double x, y;
    public:
        Point(): x(0.0), y(0.0) {}
        Point(double x0, double y0): x(x0), y(y0) {}
        Point(const Point &t): x(t.x), y(t.x) {}
        double getx() { return x; }
        double gety() { return y; } 

};

class Square: public Point {
    private:
        double l;
    public:
        Square(): Point(), l(0.0){}
        Square(double x0, double y0, double l0):Point(x0, y0), l(l0) {}
        double getl() { return l; }
        Square &operator=(Point &t) { //перевизначення присвоювання
            Square temp(t.getx(), t.gety(), 1); //сторона =1
            return *this;
        }
};

int main(int argc, char const *argv[])
{
    Point b(1,2);
    Square d(3, 4, 5);

    Point* pb;
    pb = new Point();

    delete pb; //звільнюємо аби не втратилась пам'ять
    pb = &d;

    return 0;
};
