#include <math.h>
#include <stdio.h>

double myF(double* x = 0, double* a = 0, double* b = 0) {
    if (!a || !b || !x) {
        fprintf(stderr, "ERROR: null pointer to argument(s) ( x=%p, a=%p, b=%p)", x, a, b);
        return 0;
    }
    if (*x < 5) return *a * (*x) * (*b);
        else if (*x > 5) 
            return tan(*a * (*x) * (*b));
        else 
            return *a * (*x) * (*x);  
}