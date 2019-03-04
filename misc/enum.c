#include <stdio.h>

enum A {
    ONE,
    TWO
};


int main(int argc, char *argv[])
{
    enum A a = (enum A)(ONE | TWO);

    printf("a = %#x\n", a);
    return 0;
}
