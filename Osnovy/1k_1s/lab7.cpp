#include <stdio.h>
#include "lab7_lib.h"

int main(int argc, char const *argv[])
{
    double a, b, x;
    printf("Enter arguments:\n\tx = ");
    scanf("%lf", &x);
    printf("\ta = ");
    scanf("%lf", &a);
    printf("\tb = ");
    scanf("%lf", &b);

    printf("\nResult is:\n\ty(%g, %g, %g) = %g\n", x, a, b, myF(&x, &a, &b));
    getchar();
    getchar();
    return 0;
}
