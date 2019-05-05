//#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <math.h>

using namespace std;

class DeqNode {
    double value;
public:
    DeqNode* next;
    DeqNode* last;
    DeqNode(double value=0, DeqNode* last=NULL, DeqNode* next=NULL) {
        this->value = value;
        this->last = last;
        this->next = next;
        if (next) next->last = this;
        if (last) last->next = this;
    }
    ~DeqNode (){
        remove();
    }
    double getVal() { return value; }
    DeqNode* remove() {
        if (next) next->last = last;
        if (last) last->next = next;
        return this;
    }
};

class Deq {
    DeqNode* fst;
    DeqNode* lst;
    DeqNode* cur;
public:
    Deq() {
        fst = lst = cur = NULL;
    }
    ~Deq() {
        cur = fst;
        while (cur) {
            cur = cur->next;
            delete fst;
            fst = cur; 
        }
    }
    DeqNode* append(double val, DeqNode* after=NULL) {
        if (!after) after = lst; 
        lst = cur = new DeqNode(val, after, after ? after->next : NULL);
        if (!fst) fst = lst;
        return cur;
    }

    DeqNode* remove(DeqNode* del) {
        if (!del) return NULL;
        if (fst == del) { fst = del->next; }
        if (lst == del) { lst = del->last; }
        cur = del->next;
        delete del;
        return cur;
    }

    DeqNode* makeFirst(DeqNode* del) {
        if (!del || del == fst ) return NULL;
        //getNext();
        removeWODelete(del);
        del->next = fst;
        del->last = NULL;
        fst->last = del;
        fst = del;
        return cur;
    }

    DeqNode* removeWODelete(DeqNode* del) {
        if (!del) return NULL;
        if (fst == del) { fst = del->next; }
        if (lst == del) { lst = del->last; }
        cur = del->next;
        del->remove();
        return del;
    }

    DeqNode* getFirst() { return fst; }
    DeqNode* getCurrent() { return cur; }
    DeqNode* getLast() { return lst; }
    DeqNode* getNext() {
        if (cur) cur = cur->next;
            else cur = fst; 
        return cur;
    }
    
    DeqNode* setCurrent(DeqNode* node) { cur = node; }

    void print() {
        DeqNode* tmp = fst;
        while (tmp) {
            cout.width(10); cout.fill(' ');
            cout << tmp->getVal() << "\t";
            cout.width(10); cout.fill(' ');
            cout << tmp->last << "\t";
            cout.width(10); cout.fill(' ');
            cout << tmp << "\t";
            cout.width(10); cout.fill(' ');
            cout << tmp->next << endl; 
            tmp = tmp->next;            
        }
    }

    DeqNode* swap(DeqNode* a, DeqNode* b) {
        DeqNode* tmp;
        if (a && b) { 

            tmp = a->last; 
            a->last = b->last;
            b->last = tmp;
            if (b->last) b->last->next = b;
            if (a->last) a->last->next = a;
            
            tmp = a->next;  
            a->next = b->next; 
            b->next = tmp;
            if (b->next) b->next->last = b;
            if (a->next) a->next->last = a;

            if (cur == a) cur = b;
            if (cur == b) cur = a;
            if (fst == a) fst = b;
            if (fst == b) fst = a;
            if (lst == a) lst = b;
            if (lst == b) lst = a;
        }
        return cur;
    }
};

int main(int argc, char const *argv[])
{
    Deq mydeq;
    DeqNode* tmp;

    for (int i=-3; i<5; i++) mydeq.append(floor(rand()/(RAND_MAX/3)));

    cout << "Initial deque state:" << endl;
    mydeq.print();

    cout << "Deque state with zeroes moved to begin:" << endl;
    mydeq.setCurrent(NULL);
    while (mydeq.getNext()) {
        if ( mydeq.getCurrent()->getVal() == 0 ) {
            mydeq.makeFirst(mydeq.getCurrent());
        }
    }

    cout << endl;
    mydeq.print();

    cin.get();
   return 0;
}
