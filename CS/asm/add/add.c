#include <stdio.h>


int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    int c = 0;

    __asm__ (
            "addl %2,%0"
            : "=g"(c)
            : "0"(a), "g"(b)
            : "memory");

    printf("c = %d\n", c);
    return 0;
}
