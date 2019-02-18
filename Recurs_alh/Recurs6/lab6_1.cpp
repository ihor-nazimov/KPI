#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
//#include <conio.h>
#include <string.h>
#include <errno.h>
//#include <errors.h>
#include <limits.h>

using namespace std;

//simple random tree class
class RandomTree {
public:
    RandomTree* parent = 0;
    RandomTree* firstchild = 0;
    RandomTree* nextnode = 0;
    int value = 0;

/*    RandomTree(RandomTree* parent = 0) { //create random tree node and childs
        value = rand() / (RAND_MAX >> 8);
        this->parent = parent;
        
        //if isn't root, try to add next nodes
        if (parent)
            if (rand() > (RAND_MAX >> 1) ) 
                nextnode = new RandomTree(parent);

        if (!parent) 
            firstchild = new RandomTree(this);
        else if (rand() > (RAND_MAX >> 1)) 
            firstchild = new RandomTree(this);
    }*/

    RandomTree(RandomTree* parent = 0, int value=0) { //create random tree node and childs
        this->value = value;
        this->parent = parent;
        this->firstchild = NULL;
        this->nextnode = NULL;
    }

    RandomTree* addChild(int value) {
        RandomTree* tmp = new RandomTree(this, value);
        RandomTree* last = firstchild;
        while (last && last->nextnode) 
            last = last->nextnode;
        if (last)
            last->nextnode = tmp;
        if (!firstchild) 
            firstchild = tmp;
        return tmp;
    }

    RandomTree* addNext(int value) {
        RandomTree* tmp = new RandomTree(parent, value);
        RandomTree* last = this;
        while (last && last->nextnode) 
            last = last->nextnode;
        last->nextnode = tmp;
        return tmp;
    }

    ~RandomTree() {
        delete(firstchild);
        delete(nextnode);
    }

/*    int map(int f(RandomTree*)) {

    };*/

    void mirror() {
        if (firstchild) 
            firstchild->mirror();

        RandomTree* oldnext; 
        RandomTree* oldlast = this; 
        RandomTree* cur = nextnode;
        
        while (cur) {
            oldnext = cur->nextnode;
            cur->nextnode = oldlast;
            oldlast = cur;
            if (!oldnext) {
                parent->firstchild = cur;
                nextnode = 0;
            }    
            if (cur->firstchild) cur->firstchild->mirror();
            cur = oldnext;
        }
    }

    void print(int level) {
        //printf("%d (this=%p, next=%p, child=%p)\n", value, this, nextnode, firstchild);
        printf("%d \n", value);
        if (firstchild) {
            for (int i=0; i < level; i++, printf(" "));
            printf("    "); firstchild->print(level+4);
        }
        if (nextnode) {
            for (int i=0; i < level-4; i++, printf(" "));
            if (level > 0) printf("    ");
            nextnode->print(level);
        }        
    }

};

int main(int argc, char const *argv[])
{
    RandomTree* tree = new RandomTree(NULL, 0);

    tree->addChild(1)->
            addChild(2)->
                addChild(3)->
                addNext(5)->parent->
            addNext(6)-> 
                addChild(7);

    tree->addChild(8)->
            addChild(9)->
                addChild(10)->
                addNext(11);

    //tree->firstchild

    tree->print(0);
    printf("Press ENTER to mirror the tree\n");
    getchar();

    tree->mirror();
    tree->print(0);



    getchar();
    return 0;
}
