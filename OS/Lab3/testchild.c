#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int i, n;
    char buf[1024];
    fprintf(stderr, "Testchild: before fgets\n");
    while ( fgets(buf, sizeof(buf), stdin) ) {
        buf[1023] = '\0';
        printf("Testchild: %s\n", buf);
    }
    fprintf(stderr, "Testchild: after fgets\n");
    fflush(stdout);
    fclose(stdout);
    return 0;
}