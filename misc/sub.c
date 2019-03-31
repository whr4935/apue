#include <stdio.h>


int main(int argc, char *argv[])
{
    unsigned short a = 65535;
    unsigned short b = 0;
    unsigned short c = b - a;
    printf("%d\n", c);
    if (1U != (unsigned short)(b - a)) {
        printf("not equal\n");
    }
    unsigned int d = b-a;
    printf("d = %d, %x\n", d, d);


    char c1 = 1;
    char c2 = -128;
    char c3 = c2 - c1;
    printf("c3 = %d\n", c3);
    return 0;
}
