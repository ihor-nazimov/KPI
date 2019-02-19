#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    float x, a, b;
    
    printf("Enter x =");
    scanf("%f", &x);
    printf("Enter a =");
    scanf("%f", &a);
    printf("Enter b =");
    scanf("%f", &b);
    
    printf("Result: y=");
	if (x > 5) {
		printf("%f", tan(a*x*b));
	} else if (x < 5) {
		printf("%f", a*x*b);
	} else 
		printf("%f", a * pow(x, b));
	
	printf("\nPress ENTER to exit");
	getchar();
	getchar();
	
    return 0;
}