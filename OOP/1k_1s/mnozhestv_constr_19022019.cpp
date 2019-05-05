/* для конструкторів використовуємо
???
classX: f() {}
classY: virtual public X 

без virual має сенс, якщо у кожного доч. класа свої поля такі ж самі, як у батьків */

class Date{
        int d,m,y;
    public:
        Date(int d0, int m0, int y0): d(d0), m(m0), y(y0) {}
};

class Person: public Date {
        char* name;
    public:
        Person(char* nm, int dn, int mn, int yn): Date(dn, mn, yn) {
            name = new[strlen(nm)+1];
            strcpy(name, nm);
        }
};

class Tovar: public Date { 
        int kil, double vart;
    public:
        Tovar(int kil0, double vart0, int dnd, int mnd, int ynd):
            Date()(dnd, mnd, ynd), kil(kil), vart(vart0){}
};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
