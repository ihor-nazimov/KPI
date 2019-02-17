 #include <stdio.h>
 #include <string.h>

int scansbits(char *s) {
    char a = '\0';
    for (int i = 0; i <= strlen(s); i++) {
        if (s[i] == '1') { a <<= 1; a += 1; }
        else if (s[i] == '0') { a <<= 1; }
            else break;
    };
    return a;
}

int main(int argc, char const *argv[])
{
    char s[300];
    char* w = 0;
    int n=0;
    char mask[]="01";
    printf("Enter bits string:\n");
    scanf("%[01 ,]s", &s);
    w = strtok(s, " ,\n"); 
    do {
        printf("%c", scansbits(w));
    } while ( w = strtok(0, " ,\n") );
    getchar(); getchar(); getchar(); 
    
    return 0;

}
