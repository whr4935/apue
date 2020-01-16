#include <stdio.h>

struct A {
    int a;
};

struct B {
    int b;
};

union UType {
    struct A a;
    struct B b;
};

int main(int argc, char *argv[])
{
    union UType u;

    printf("%p %p %p", &u, &u.a, &u.b);


    return 0;
}
