#include <stdio.h>

class B {
public:
    int value;
    B* left;
    B* right;
    B* parent;

    B(int value, B* parent) {
        this->value = value;
        this->parent = parent;
        left = right = 0;
    }

    B* addleft(int value) {
        if (!left)
            left = new B(value, this);
        return left;
    }
    B* addright(int value) {
        if (!right)
            right = new B(value, this);
        return right;
    }
    void print(int level) {
        if (left) left->print(level+1);
        for (int i = 0; i<level; i++) printf("   ");
        printf("%d\n", value);
        if (right) right->print(level+1);
    }
    
};

int main(int argc, char const *argv[])
{
    B* root = new B(8, 0);
    
    root->addleft(3)->
        addleft(1)->parent->
        addright(6)->
            addleft(4)->parent->
            addright(7);
    root->addright(10)->
        addright(14)->
            addleft(13);

    root->print(0);

    getchar();getchar();
    return 0;
}


