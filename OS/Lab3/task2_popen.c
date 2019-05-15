#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
    const int MAXLINE = 4096;
    FILE* fp;
    char buf[MAXLINE];
    sprintf(buf, "who");
    if ((fp = popen(buf, "r")) == NULL)
    {
        fprintf(stderr, "Can't call popen: %s\n", strerror(errno));
        exit(1);
    }
    /* зчитування з каналу IPC, запис у стандартний потік виведення */
    while (fgets(buf, MAXLINE, fp) != NULL)
        if (fputs(buf, stdout) == EOF) {
            fprintf(stderr, "Can't output to stdout: %s\n", strerror(errno));
            exit(1);
        }
    if (ferror(fp)) {
        fprintf(stderr, "Error while reading from pipe : %s\n", strerror(errno));
        exit(1);
    }
    pclose(fp);
    return 0;
}
