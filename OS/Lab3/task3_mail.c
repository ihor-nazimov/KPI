#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
    const int MAXLINE = 4096;
    FILE* fp;
    char buf[MAXLINE];
    sprintf(buf, "mail -s \"Test Subject4\" -r \"ihor.nazimov@gmail.com\" user1\0");
    printf("Calling command: %s\n", buf);
    if ((fp = popen(buf, "w")) == NULL)
    {
        fprintf(stderr, "Can't call popen: %s\n", strerror(errno));
        exit(1);
    }
    /* зчитування з каналу IPC, запис у стандартний потік виведення */
    sprintf(buf, "Hi!\n It's test message.\nBye!\n");
    printf("Sending text:\n%s\n", buf);
    if (fputs(buf, fp) <= 0 ) {
        fprintf(stderr, "Can't put message to mail: %s\n", strerror(errno));
        exit(1);
    }
    if (pclose(fp) == -1) {
        fprintf(stderr, "Can't close command: %s\n", strerror(errno));
        exit(1);
    };
    printf("Mail was sent\n", buf);

    return 0;
}
