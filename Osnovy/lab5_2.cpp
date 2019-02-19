#include <stdio.h>
#include <math.h>

double f1(double x, double a, double b);

int main(int argc, char const *argv[])
{
    double x, a, b;

    if (argc > 3 && 
        sscanf(argv[1], "%lf", &x) > 0 &&
        sscanf(argv[2], "%lf", &a) > 0 &&
        sscanf(argv[3], "%lf", &b) > 0 ) {
            printf("Result: y ( x:%g, a:%g, b:%g ) = %g\n", x, a, b, f1(x, a, b));
            getchar();
            return 0;
    } 

    printf("Incorrect input.\nUse:\n  lab4 x a b\nwhere x, a, b are arguments\n");
    getchar();
    return -1;
}

double f1(double x, double a, double b) {
	if (x > 5)
		return tan(a*x*b);
	else if (x < 5) 
		return a*x*b;
	else 
		return a * pow(x, b);
}

