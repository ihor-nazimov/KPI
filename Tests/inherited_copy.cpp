#include <iostream>
#include <cstring>

using namespace std;

class A {
    protected:
        int afld=1;
        char* s=0;
    public:
        A() {
            s = new char[5];
            strcpy(s, "test");
        }
        A(int ss): A() { afld = ss*3; }
        A(const A& src) {
            afld = 2*src.afld;
            s = new char[ strlen(src.s) ];
            strcpy(s, src.s);
        }
};

class C: public A {
    public:
        C() {}
        C(int ss): A(ss) {}
        C(const C& src): A(src) {
            afld = 4*src.afld;
        }
        char* retchars(const char* src) {
            char* s = new char [strlen(src)*2+1];
            strcpy(s, src);
            return s;
        }
        int retval() { return afld; }
};

int main(int argc, char const *argv[])
{
    C* c1 = new C();
    C* c2 = new C(*c1);
    C* c3 = new C();
    *c3 = *c2;
    C* nil = 0;

    C st(1900);
    cout << st.retval() << endl;
    if ( c3 ) {
        cout << "OK" << endl;
    };
    if ( &c3 ) {
        cout << "OK" << endl;
    };
    if ( nil ) {
        cout << "OK" << endl;
    };
    if ( &nil ) {
        cout << "OK" << endl;
    };


    char* a = c3->retchars("abc");
    char* b = c3->retchars("def");

    return 0;
}

