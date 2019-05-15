#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    #if defined _WIN64 || defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    printf("Windows\n");
    #endif
    
    #if defined __linux__
    printf("Linux\n");
    #endif

    #if defined linux || defined __linux
    printf("Linux (not POSIX)\n");
    #endif

    #if defined __APPLE__ || defined __MACH__
    printf("Mac OS X\n");
    #endif

    #if defined unix || defined __unix || defined __unix__
    printf("Unix (Linux, * BSD, Mac OS X)\n");
    #endif

    #if defined __FreeBSD__
    printf("FreeBSD\n");
    #endif

    getchar();
    getchar();
    return 0;
}
