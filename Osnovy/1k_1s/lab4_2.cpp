#include <stdio.h>
#include <math.h>

void f1();

int main(int argc, char const *argv[])
{
    double x, a, b;

    if (argc > 3 && 
        sscanf(argv[1], x) > 0 ) &&
        sscanf(argv[2], a) > 0 ) &&
        sscanf(argv[3], b) > 0 ) ) {
            printf("Result: y ( x:%g, a:%g, b:%g ) = %g", x, a, b, f1(x, a, b));
            return 0;
    } 

    printf("Incorrect input.\nUse:\n  lab4 x a b\nwhere x, a, b are arguments\n");
    getchar();
}

double f1(double x, double a, double b) {
	if (x > 5)
		return tan(a*x*b);
	else if (x < 5) 
		return a*x*b;
	else 
		return a * pow(x, b);
}

