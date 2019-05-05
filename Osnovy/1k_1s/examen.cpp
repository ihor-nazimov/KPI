#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE* fp;
    FILE* fp2;
    char fname[] = "lab9.exe";
    fp = fopen(fname, "r");
    fp2 = fopen("lab9.exe.bak", "w");

    char tmp[1024];
    size_t sz;

    while (!feof(fp)) {
        //tmp = fgetc(fp);
        //if (!feof(fp)) fputc(tmp, fp2);
        sz = fread((char*) &tmp, 1, sizeof(tmp), fp);
        fwrite((char*) &tmp, 1, sz, fp2);
    }

    fclose(fp);
    fclose(fp2);
    return 0;
}
