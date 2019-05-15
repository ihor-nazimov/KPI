/*Лабораторная работа № 3

Демонстрация взаимодействия процессов с использованием каналов

Задание 1. Написать программу, запускающую процесс-потомок с заменой его стандартного ввода-вывода. Затем программа должна передать процессу-потомку информацию на стандартный ввод и вывести на экран его стандартный вывод. В качестве программного кода для процессов потомков следует использовать программы-фильтры типа head, tail, sort, wc, grep, awk. На вход процессу-потомку следует передать содержимое произвольного текстового файла.
Задание 2. С помощью функции popen запустить программу, например, who, ps, ifconfig, route, uname, df и вывести содержимое ее стандартного вывода на экран. Кроме того, с помощью той же функции и команды mail отправить поздравительные письма указанным в командной строке адресатам. */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//char *a = "asdfs";
//char* env_init[] = { NULL };
//har* args[] = {"sort", NULL };
int P2C[2], //parent -> child
    C2P[2];   //child -> parent

void closeonexit(void) {
    close(P2C[0]); //close write 
    close(C2P[0]); //close read
    close(P2C[1]); //close write 
    close(C2P[1]); //close read

    close(0);
    close(1);
}

void child_for_out(int c2p) {
    char outbuf[1024];
    int n;
    while ((n = read(c2p, outbuf, sizeof(outbuf))) != 0 ) {
      if (outbuf[n-1] == '\n') outbuf[n-1] = '\0'; 
      printf("Command output:\n%s\n", outbuf);
    }
    if (n == -1) {
        fprintf(stderr, "Can't print command output: %s\n", strerror(errno));
        exit(-1);
    }
    close(c2p);
    //printf("End of child_for_output\n");
}

int main(int argc, char** argv) {
    pid_t pid, pidCP;
    FILE* fin;
    int mode;
    char command[256];
    char commandkey[256] = "";
    //  int fout;
    int n;
    char buf[1024];

    if (argc != 2) {
      fprintf(stderr, "Usage: fork_exec <file>\n");
      exit(1);
    }

    printf("Select command:\n1 - head\n2 - tail\n3 - wc\n4 - grep ...\n5 - awk ...\n0 - exit\n");
    scanf("%d", &mode);

    switch (mode) {
        case 0: exit(0);
        case 1: strcpy(command, "head"); break;
        case 2: strcpy(command, "tail"); break;
        case 3: strcpy(command,"wc"); break;
        case 4: 
            strcpy(command, "grep"); 
        case 5:
            if (mode == 5) strcpy(command, "awk"); 
            printf("Enter argument: ");

            scanf("%s", commandkey);
            //if (!fgets(commandkey, sizeof(commandkey), stdin)) { //it doesn't stop on fgets
            if (strlen(commandkey) == 0) {
                fprintf(stderr, "Incorrect key: %s\n", strerror(errno));
                //fprintf(stderr, "Usage: fork_exec <file>\n");
                exit(1);
            };
            break;
        default:
            fprintf(stderr, "Usage: fork_exec <file>\n");
            exit(1);
    }

    printf("Try to run command: %s %s\n", command, commandkey);
    
    if ( atexit(closeonexit) != 0 ) {
        fprintf(stderr, "Помилка виклику функції atexit: %s\n", strerror(errno));
        exit(1);
    }

    if (pipe(P2C)) {
        fprintf(stderr, "Error while opening parent-child pipe: %s\n", strerror(errno));
        exit(1);
    }
    
    if (pipe(C2P)) {
        fprintf(stderr, "Error while opening child-parent pipe: %s\n", strerror(errno));
        exit(1);
    }

    //START CHILD FOR COMMAND OUTPUT CATCHING
    if ( (pidCP = fork()) < 0) {
        fprintf(stderr, "Помилка виклику функції fork (C>>P): %s\n", strerror(errno));
        exit(1);
    } else if (pidCP == 0) {
        //close all except reading from command child
        //printf("Child for out: before stdio redirecting\n");
        close(P2C[0]); 
        close(P2C[1]); 
        close(C2P[1]); 
        // //close(0); //stdin
        // dup2(, 0); //redirect stdin
        // close(C2P[0]); //it's don't need pipe descriptor anymore 
        // printf("Child for out: after stdio redirecting\n");
        child_for_out(C2P[0]);
        //printf("Child for out: before exit\n");
        exit(0);
    }

    //CHILD FOR COMMAND
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Помилка виклику функції fork (P>>C): %s\n", strerror(errno));
        exit(1);
    }      
    else {
    //printf("After fork: pid=%p\n", pid);
    //args[0] = "int";
    if (pid == 0) {/* задати повний шлях до файлу й оточення */
      //in child process
      //printf("Child for command: before stdio redirecting\n");
      close(P2C[1]); //close write P>>C 
      close(C2P[0]); //close read C>>P

      //close(0); //stdin
      dup2(P2C[0], 0); //redirect stdio to P>>C
      close(P2C[0]); //don't need anymore

      //close(1); //stdout
      dup2(C2P[1], 1); //redirect stdout to C>>P
      //dup2(C2P[1], 2); //redirect stderr to C>>P
      close(C2P[1]); //don't need anymore

      //fprintf(stderr, "Child for command: before exec...\n");
      //if (execvp("/home/user1/KPIRepo/OS/Lab3/testchild.out", args) )
      int rslt;
      switch (mode) {
          case 4:
            rslt = execlp(command, command, commandkey, NULL); 
            break;
          default:
            rslt = execlp(command, command, NULL);
      } 

      if (rslt)//, env_init) )
        {
        fprintf(stderr, "Помилка виклику функції execle: %s\n", strerror(errno));
        exit(1);
        } 
      //fprintf(stderr, "Child for command: before exit(1)\n");
      exit(1);
    }   

    //PARENT
    //fprintf(stderr, "Parent: after child if\n");
      
    close(P2C[0]); //close read from parent 
    close(C2P[1]); //close write to parent
    close(C2P[0]); //close read from child - child_for_out does it

    if ( (fin = fopen(argv[1], "r")) == NULL ) {
      fprintf(stderr, "Can't open source file: %s\n", strerror(errno));
      exit(1);
    };

    do {
        n = fread(buf, 1, sizeof(buf), fin);
        if (ferror(fin)) {
            fprintf(stderr, "Can't read source file: %s\n", strerror(errno));
            exit(1);
        }
        if (write(P2C[1], buf, n) <= 0) {
            fprintf(stderr, "Can't write to command input pipe: %s\n", strerror(errno));
            exit(1);
        };
    } while (!feof(fin));
    fclose(fin);

    //strncpy(buf, "fasdfsdabca;slkfj\n;alksgrepjf;laskdj\n;laksd;jlfkjdf\nqwertabcdsfv\n\0", sizeof(buf));
    //printf("Parent: write %d \n", write(P2C[1], buf, strlen(buf)+1));
    //flush(P2C[1]);
    close(P2C[1]);
     /*if ( n=write(P2C[1], buf, sizeof(buf)) > 0 ) {
        fprintf(stderr, "Помилка виклику функції write: %s\n", strerror(errno));
        close(P2C[1]); //close write 
        close(C2P[0]); //close read
        exit(1);
    } ;*/
    
  }
  //in parent process  
  //fprintf(stderr, "Parent: before wait\n");
      
  if (waitpid(pid, NULL, 0) < 0)
     {
      fprintf(stderr, "Помилка виклику функції waitpid for command: %s\n", strerror(errno));
      exit(1);
     }    

  if (waitpid(pidCP, NULL, 0) < 0)
     {
      fprintf(stderr, "Помилка виклику функції waitpid for output: %s\n", strerror(errno));
      exit(1);
     }    
    //fprintf(stderr, "Parent: before exit\n");
  
  getchar();//getchar();
  exit(0);
 }
