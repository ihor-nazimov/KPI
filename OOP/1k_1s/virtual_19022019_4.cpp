#include <stdio.h>

class Base {
    protected:
        int x;
    public:
        Base(int x0=10) { x = x0; }
        //якщо virual - пізнє зв'язування. Методи викликаються згідно типу об'єкта
        //без virtual - викликається згідно типу вказівника
        //virtual void modifyx() { x *= 2; } //за дійсним типом об'єкта
        void modifyx() { x *= 2; } //викликається метод за типом вказівника

        void show() { printf("x=%d\n", x); }
};

class Derived: public Base {
    void modifyx() {x /= 2; }

};

int main(int argc, char const *argv[])
{
    int mode;
    Base *pb; 
 
    printf("1=Base, else Derived\n");
    scanf("%d", &mode);
    //раннє зв'язування. На час компіляції невідомо з яким класом буде зв'язаний.
    //якщо не відомо, то за типом вказівника.
     if (mode==1) pb = new Base;
        else pb = new Derived;
    pb->modifyx();pb->show();

    //буде викликаний згідно раннього зв'язування
    Base b;
    Derived d;
    pb = &b; pb->modifyx(); pb->show(); //20
    pb = &d; pb->modifyx(); pb->show();//20  

    getchar();getchar();
    //якщо "virtual" буде викликаний згідно пізнього зв'язування

};
