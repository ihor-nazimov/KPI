#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
//#include <conio.h>
#include <string.h>
#include <errno.h>
//#include <errors.h>
#include <limits.h>

using namespace std;
#define STACKSIZE 100

//simple LIFO stack
class Stack {
    int* stackmem;
    int stacksize;
    int* current = 0;

public:
    Stack(int sz = STACKSIZE) {
        if ((stackmem = new int[sz]) <= 0) {
            //fprintf(stderr, "ERROR: can't init stack memory (%s)\n", strerror(errno));
            //return nullptr;
        }   
        stacksize = sz;
        current = stackmem-1;
    }

    ~Stack() {
        delete(stackmem);
    }

    int push(int a) { //returns 0 if OK
        if (current - stackmem >= stacksize-1) {
            //fprintf(stderr, "ERROR: stack overflow\n");
            return -1;
        } 
        *(++current) = a;
        return 0;
    }

    int pop(int* a) { //gets stack into a and returns 0 if OK. 
        if (current < stackmem) {
            //fprintf(stderr, "ERROR: stack empty\n");
            return -1;
        } 
        *a = *(current--);
        return 0;
    }

    void print() {
        for (int* ptr = stackmem; ptr <= current; ptr++) {
            printf("%d\n", *ptr);
        }
    }

};

int main(int argc, char const *argv[])
{
    Stack* stk = new Stack(150);
    
    for (int i=0; i<5; i++) stk->push( 10*(rand() - (RAND_MAX >> 1))/RAND_MAX );
    printf("Stack state before processing:\n");
    stk->print();
    
    //create temp. stack to find and remove minimal value
    Stack* tmp = new Stack(150);
    int minval=INT_MAX;
    int buf;
    while (!stk->pop(&buf)) { 
        if (buf > 0 && minval > buf) minval = buf;
        tmp->push(buf);
    }

    while (!tmp->pop(&buf)) { 
        if (buf != minval) stk->push(buf);
    }

    printf("Stack state after processing:\n");
    stk->print();
    
    delete(stk);

    getchar();
    //getchar();
    return 0;
}
