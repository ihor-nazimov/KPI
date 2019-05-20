#include <iostream>
#include<cstdio>

class CTst {
    public:
        int a=0;
        CTst(int aa) {
            a = aa;
        }
        ~CTst() {
            int b = 0;
        }
};

void test(CTst& t){
    std::cout << t.a << " " << (&t) << std::endl;
}

int main(int argc, char const *argv[])
{
    CTst* tmp;
    CTst* tst = new CTst(123);
    test(*tst);
    test(*(tmp = new CTst(456))); delete tmp;
    test(*(tmp = new CTst(789))); delete tmp;
    test(*(tmp = new CTst(007))); delete tmp;

    delete tst;
    getchar();
    return 0;
}
