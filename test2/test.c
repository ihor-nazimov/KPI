 #include <stdio.h>

int test() {
    char ss[10] = "345";
    char* s2;
    printf("%s\n", s2);
    s2 = ss;
    printf("%s\n", s2);
}

int main(int argc, char const *argv[])
{
    char* s = "1234";
    printf("%s\n", s);
    test();
    getchar();
    return 0;
}
