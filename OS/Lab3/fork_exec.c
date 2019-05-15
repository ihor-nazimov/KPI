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
char*	env_init[] = { NULL };
char* args[] = {"grep", NULL };
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

int main(void)
 {
  pid_t pid;
//  int fin;
//  int fout;
  int n;
  char buf[1024];

  // printf("1\n");
  //getchar();

  if ( atexit(closeonexit) != 0 ) {
    fprintf(stderr, "Помилка виклику функції atexit: %s\n", strerror(errno));
    exit(1);
  }

  pipe(P2C);
  pipe(C2P);
  if ((pid = fork()) < 0)
   {
    fprintf(stderr, "Помилка виклику функції fork: %s\n", strerror(errno));
    exit(1);
   }      
  else {
    printf("After fork: pid=%p\n", pid);
    //args[0] = "int";
    if (pid == 0) {/* задати повний шлях до файлу й оточення */
      //in child process
      printf("Child: before stdio redirecting\n");
      close(P2C[1]); //close write 
      close(C2P[0]); //close read
      //close(0); //stdin
      dup2(P2C[0], 0);
      close(P2C[0]);

      //close(1); //stdout
      dup2(C2P[1], 1);
      dup2(C2P[1], 2);
      close(C2P[1]);
      printf("Child: after stdio redirecting\n");

      fprintf(stderr, "Child: before exec...\n");
      //if (execve("/home/user1/KPIRepo/OS/Lab3/testchild.out", args, env_init) )
      if (execve("/usr/bin/grep", args, env_init) )
        {
        fprintf(stderr, "Помилка виклику функції execle: %s\n", strerror(errno));
        exit(1);
        } 
      fprintf(stderr, "Child: before exit(1)\n");
      exit(1);
    }   
    fprintf(stderr, "Parent: after child if\n");
      
    close(P2C[0]); //close write 
    close(C2P[1]); //close read
/*    if ( (fin = fdopen(P2C[1], "r")) == 0) {
        fprintf(stderr, "Помилка виклику функції fdopen(P2C): %s\n", strerror(errno));
        exit(1);
    };
    if ( (fout = fdopen(C2P[0], "w")) == 0) {
        fprintf(stderr, "Помилка виклику функції fdopen(C2P): %s\n", strerror(errno));
        exit(1);
    };*/

//    printf("1/ From child: %s\n", fgets(buf, sizeof(buf), fin));
    //snprintf(buf, sizeof(buf), "Write to child"); 
    /*if ( n=write(P2C[1], buf, sizeof(buf)) > 0 ) {
        fprintf(stderr, "Помилка виклику функції write: %s\n", strerror(errno));
        close(P2C[1]); //close write 
        close(C2P[0]); //close read
        exit(1);
    } ;*/
    while (read(C2P[0], buf, sizeof(buf)) != 0 ) {
      printf("Parent. Buf:%s\n", buf);
    }

    /*if ( n= > 0) {
        fprintf(stderr, "Помилка виклику функції read: %s\n", strerror(errno));
        exit(1);

    };*/
    //fgets(buf, sizeof(buf), fout);
    
    printf("Parent: buf=%s\n", buf);

    //printf("3\n");

    //args[0] = "3";
  }
  //in parent process  
  fprintf(stderr, "Parent: before wait\n");
      
  if (waitpid(pid, NULL, 0) < 0)
     {
      fprintf(stderr, "Помилка виклику функції waitpid: %s\n", strerror(errno));
      exit(1);
     }    
    fprintf(stderr, "Parent: before exit\n");
  
  getchar();getchar();
  exit(0);
 }
