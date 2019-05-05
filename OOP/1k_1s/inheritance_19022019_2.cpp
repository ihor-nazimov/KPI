#include <stdio.h>

class Base {
    public:
        void show() {
            printf("Base class\n");
        }
};

class Derived: public Base {
    public:
        void show() {
            Base:show();
            printf("Derived\n");
        }
};

class Subderived: public Derived {
    public:
        void show() {
            Derived:show(); //ошибка - не входит в функцию!
            printf("Subderived\n");
        }
};

int main(int argc, char const *argv[])
{
    Subderived sub;

    sub.show();
    return 0;
}
