#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned char a , b;

    a = 255;
    b = 1;

    unsigned char c = b - a;
    short d = b -a;
    printf("c = %d, d = %d\n", c, d);
    return 0;
}
