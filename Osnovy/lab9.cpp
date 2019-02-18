#include <stdio.h>
#include <string.h>
#include "lab7_lib.h"

double myF2(double* x, double* a, double* b) {
    if (!a || !b || !x) {
        fprintf(stderr, "ERROR: null pointer to argument(s) ( x=%p, a=%p, b=%p)", x, a, b);
        return 0;
    }
    return ( log10(*a + *x**x) - sqrt(*b**x) ) / 
				( tan(*b**x) + pow( fabs(*a + *x), .2 ) ) ;
}

int main(int argc, char const *argv[])
{
    struct {
        double x, a, b, y;
        char date[12], groupIndex[10];
        char* studentName; 
    } myStruct;

    strcpy(myStruct.date, "18.02.2019");
    strcpy(myStruct.groupIndex, "ZPI-zp83");
    strcpy( ( myStruct.studentName = new char[255]), "Ihor Nazimov");

    printf("Enter arguments:\n\tx = ");
    scanf("%lf", &myStruct.x);
    printf("\ta = ");
    scanf("%lf", &myStruct.a);
    printf("\tb = ");
    scanf("%lf", &myStruct.b);

    myStruct.y = myF2(&myStruct.x, &myStruct.a, &myStruct.b);

    printf("\nResult is:\n\ty(%g, %g, %g) = %g\n", myStruct.x, myStruct.a, myStruct.b, myStruct.y);

    printf("\nmyStruct.date:%s\n", myStruct.date);
    printf("myStruct.groupIndex:%s\n", myStruct.groupIndex);
    printf("myStruct.studentName:%s\n", myStruct.studentName);

    char fname[256];
    FILE* fo;
    do
    {
        printf("\nEnter filename: ");
        scanf("%255s", fname);
        fo = fopen(fname, "wa+t");
        if (!fo) printf("Can't open or create file %s", fname);    
    } while (fo == NULL);
    
    //Name - 20, Value - 20
    fprintf(fo, "       Name          |      Value\n");
    fprintf(fo, " myStruct.x          | %g\n", myStruct.x);
    fprintf(fo, " myStruct.a          | %g\n", myStruct.a);
    fprintf(fo, " myStruct.b          | %g\n", myStruct.b);
    fprintf(fo, " myStruct.y          | %g\n", myStruct.y);
    fprintf(fo, " myStruct.date       | %s\n", myStruct.date);
    fprintf(fo, " myStruct.groupIndex | %s\n", myStruct.groupIndex);
    fprintf(fo, " myStruct.studentName| %s\n", myStruct.studentName);

    fclose(fo);

    delete myStruct.studentName;

    printf("Data saved in file %s\n", fname);

    getchar();
    getchar();
    return 0;
}
