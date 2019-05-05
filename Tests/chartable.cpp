#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    unsigned short asc;
    for (int i=-1; i<16; i++) {
        if (i < 0) printf("   ");
            else printf("%2x ", i*16);
        for (char j=0; j<16; j++) {
            asc = j + i*16;
            if (i<0) printf(" %1x", j);
                else if (asc > 15) 
                        printf(" %c", asc);
                    else printf("  ");
        }
        printf("\n");
    }

    getchar(); 
    return 0;
}
