#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//const double a = 1;
//const double b = 2;

typedef struct {
    double a, b, dx, x1, x2;
    double* fx;
} FXStructure;

double mapFX(FXStructure *fxstruct, double (* test) (FXStructure*, long int, double, double)){
    double x;
    double s = 0;
    for (long int i = 0; i < (long int) ((fxstruct->x2-fxstruct->x1)/fxstruct->dx + 1); i++) {
        x = i * fxstruct->dx + fxstruct->x1;
        s = test(fxstruct, i, x, s);
    } 
    return s;       
}

double f(double x, double a, double b) {
    if (x > 0.6) {
        return -a*x*x+b;
    } else 
        return b/x;
}

double cb_fill(FXStructure *fxstruct, long int i, double x, double s) {
    fxstruct->fx[i] = f(x, fxstruct->a, fxstruct->b);
}

#if defined __linux__
double cb_printTabString(FXStructure *fxstruct, long int i, double x, double s) {
    printf("\u2551 %6.1lf %10.4lf %4li \u2551\n", x, fxstruct->fx[i], i);
//    printf("\u2551 %20lg %20lg %4li \u2551\n", x, fxstruct->fx[i], i);
}
void Tab(FXStructure *fxstruct){
    printf("\u2554");
    for (int i = 0; i < 24; i++) { printf("\u2550"); }
    printf("\u2557\n");
    printf("\u2551     x       f(x)     N \u2551\n");
    mapFX(fxstruct, cb_printTabString);
    printf("\u255a");
    for (int i = 0; i < 24; i++) { printf("\u2550"); }
    printf("\u255d\n");
}
#else
double cb_printTabString(FXStructure *fxstruct, long int i, double x, double s) {
    printf("\xba %6.1lf %10.4lf %4li \xba\n", x, fxstruct->fx[i], i);
}
void Tab(FXStructure *fxstruct){
    printf("\xc9");
    for (int i = 0; i < 24; i++) { printf("\xcd"); }
    printf("\xbb\n");
    printf("\xba     x       f(x)     N \xba\n");
    mapFX(fxstruct, cb_printTabString);
    printf("\xc8");
    for (int i = 0; i < 24; i++) { printf("\xcd"); }
    printf("\xbc\n");
}
#endif

void CalculateS1S2(FXStructure *fxstruct, double* s1, double* s2) {
    double x;
    long int k = 0;
    if (!s1 && !s2) {
        fprintf(stderr, "CalculateS1S2: null pointer for s1 or s2\n");
        exit(-1);
    }
    *s1 = *s2 = 0;

    for (long int i = 0; i < (long int) ((fxstruct->x2-fxstruct->x1)/fxstruct->dx + 1); i++) {
        x = i * fxstruct->dx + fxstruct->x1;
        if (fxstruct->fx[i] > 0) 
            *s1 += fxstruct->fx[i];
        if (!(i % 2)) {
            *s2 += fxstruct->fx[i];
            k++;
        }
    }
    if (k) *s2 /= k;
}

int f2intA(double f, double fmin, double fmax, double A) {
    return (int) ((f - fmin)/(fmax-fmin)*A);
}

double cb_fmin(FXStructure *fxstruct, long int i, double x, double s) {
    if (!i) s = fxstruct->fx[i];
    return (fxstruct->fx[i] < s) ? fxstruct->fx[i] : s;
}

double cb_fmax(FXStructure *fxstruct, long int i, double x, double s) {
    if (!i) s = fxstruct->fx[i];
    return (fxstruct->fx[i] > s) ? fxstruct->fx[i] : s;
}

void Print(int* arr, long int sz) {
    for (long int i = 0; i < sz; i++){
        for (int k = 1; k < arr[i]; k++) printf(" ");
        printf("%c\n", 0xfd);
    }
}

int main(int argc, char const *argv[])
{
    long int fx_size;

    FXStructure fxstruct;

    fxstruct.a = 1;
    fxstruct.b = 2;

    printf("Enter x1 (0.1): ");
    do ; while (scanf("%lg", &fxstruct.x1) < 1);

    printf("Enter x2 (3.6): ");
    do ; while (scanf("%lg", &fxstruct.x2) < 1);
    
    printf("Enter dx: (0.1)");
    do ; while (scanf("%lg", &fxstruct.dx) < 1);

    printf("x1=%lg x2=%lg dx=%lg\n", fxstruct.x1, fxstruct.x2, fxstruct.dx);

    if (fxstruct.x1 > fxstruct.x2) {
        printf("Error: x1 value more than x2\n");
        return -1;
    }

    if ( !(fxstruct.dx > 0)) {
        printf("Error: dx value less or equal then zero\n");
        return -1;
    }

    fx_size = (long int) ((fxstruct.x2-fxstruct.x1)/fxstruct.dx + 1);
    printf("Try to allocate memory for %i doubles\n", fx_size);
    fxstruct.fx = new double [ fx_size ];  
    if ( !fxstruct.fx) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(-1);
    };

    mapFX(&fxstruct, cb_fill);

    Tab(&fxstruct);

    double s1, s2;
    CalculateS1S2(&fxstruct, &s1, &s2);
    printf("S1 = %lg \nS2 = %lg\n", s1, s2);

    double smin = (s1 < s2) ? s1 : s2;
    double smax = (s1 < s2) ? s2 : s1;
    double step = 0.025*(smax+smin)/2;
    printf("step = %lg\n", step);

    double* rnd = new double [ fx_size ];  
    if ( !rnd) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(-1);
    };

    printf("    N     rnd[i]   (rnd[i]-x1)/step\n");
    for (long int i = 0; i < fx_size; i++) {
        rnd[i] = floor((smax-smin)*rand()/RAND_MAX/step)*step + smin;  
        printf("%5li %10.5lf %10.5lf\n", i, rnd[i], (rnd[i]-smin)/step);
    }

    double fmin = mapFX(&fxstruct, cb_fmin);
    double fmax = mapFX(&fxstruct, cb_fmax);
    printf("fmin=%lg  fmax=%lg\n", fmin, fmax);

    int* arr = new int [ fx_size ];  
    if ( !arr) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(-1);
    };

    printf("    N     [0,A]\n");
    for (long int i = 0; i < fx_size; i++) {
        arr[i] = f2intA(fxstruct.fx[i], fmin, fmax, 60);  
        printf("%5li %10i\n", i, arr[i]);
    }

    Print(arr, fx_size);
    getchar();
    getchar();
    return 0;
}
