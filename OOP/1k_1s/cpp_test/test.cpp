#include <stdio.h>

template <class CType, typename TName>
    CType min(CType *m, TName sz) {
        CType min = new CType;
        min.i = sz;
        return min;
    }

class Test {
    int i;

    void init(int iii) { 
        i = iii;
    }
    Test min(Test, int);
public:
    Test(int ii=0) { 
        init(ii);
    } 
    Test(const Test &i_orig) {
        *this = i_orig;
        //init(-1);
    }
    void seti(int i) { this->i = i;  };
    Test& operator++() {
        i++;
        return *this;
    }
 
};

int main(int argc, char const *argv[])
{
    //comment
    Test obj;

    obj.seti(123);
    
    //++obj;

    Test p(++obj);
    //p.Test(-1);
    

    //delete obj;
    //delete p;
    return 0;
}