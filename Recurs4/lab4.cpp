//#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>

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
        if (next) next->last = last;
        if (last) last->next = next;
    }
    double getVal() { return value; }
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

    for (int i=-3; i<5; i++) mydeq.append(i);

    cout << "Deq state:" << endl;
    mydeq.print();

    cout << "Enter key: ";
    double key;
    cin >> key;
    cout << "Key is " << key << endl;

    mydeq.setCurrent(NULL);
    while (mydeq.getNext()) {
        if ( mydeq.getCurrent()->getVal() == key ) {
            if (mydeq.getCurrent() && mydeq.getCurrent()->next)
                mydeq.swap( mydeq.getCurrent()->next, mydeq.getCurrent()->next->next );
            mydeq.remove( mydeq.getCurrent() );
            break;
        }
    }

     mydeq.setCurrent(NULL);
    while (mydeq.getNext()) {
        if ( mydeq.getCurrent()->getVal() < 0 ) {
            mydeq.append(0, mydeq.getCurrent());
        }
    }

    cout << endl;
    mydeq.print();

    cin.get();
    cin.get();
    return 0;
}
