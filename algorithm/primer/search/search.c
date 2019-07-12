
#include "elementary_search.h"

union U{
    struct S {
        int a : 4;
        int b : 3;
        int c : 7;
    } __attribute__((packed)) s;

    struct S2 {
        int c : 7;
        int b : 3;
        int a : 4;
    } __attribute__((packed)) s2;

    short d;
};



int test_search(int argc, char **argv)
{

//    printf("sizeof of U:%d\n", sizeof(union U));

//    union U u;
//    u.d = 0x1234;
//    printf("%d %d %#x\n", u.s.a, u.s.b, u.s.c);

//    u.d = 0x3412;
//    printf("%d %d %#x\n", u.s2.a, u.s2.b, u.s2.c);


    test_elementary_search();

    test_rbtree_search();
}
