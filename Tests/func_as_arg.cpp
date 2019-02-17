#include <stdio.h>

int foo(int a) {
    return a*2;
}

int map(int a, int b(int)) {
    return b(a);
}

int main(int argc, char const *argv[])
{
    int abc = 3;
    abc = foo(abc);
    abc = map (abc, foo);
    return 0;
}
