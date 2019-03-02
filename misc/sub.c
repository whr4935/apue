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


    return 0;
}
