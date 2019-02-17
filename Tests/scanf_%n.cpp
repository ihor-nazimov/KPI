
#include <stdio.h>
int main(int argc, char const *argv[])
{
    char s[100];
    char* p = s;
    char w[100];
    int n;
    fgets(s, sizeof(s), stdin);
    printf("%d  %s  %d\n", sscanf(p, "%s%n", &w, &n), w, n);
    p+=n;
    printf("%d  %s  %d\n", sscanf(p, "%s%n", &w, &n), w, n);
    p+=n;
    printf("%d  %s  %d\n", sscanf(p, "%s%n", &w, &n), w, n);
    getdelim(w, )
    
    getchar();
    getchar();
    getchar();
    getchar();
     return 0;
}
