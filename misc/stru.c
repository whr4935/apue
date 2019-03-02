#include <stdio.h>

struct A {
    int a;
    char b;
    short c;
    int a2;
    char b2;
    short c2;
    char b3;
};

struct A ga;

int main(int argc, char *argv[])
{
    /*int a[3];*/
    /*printf("array a[1]:%d, a[2]:%d\n", a[1], a[2]);*/
    struct A la1;

    printf("la1.a:%d, la1.b:%d, la1.c:%d\n", la1.a, la1.b, la1.c);
    printf("ga.a:%d, ga.b:%d, ga.c:%d\n", ga.a, ga.b, ga.c);
    printf("sizeof %d\n", sizeof(struct A));
    
    return 0;
}
