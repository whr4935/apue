#include <stdio.h>

int main(int argc, char *argv[])
{
    double pi = 3.14;
    int pi2 = (int)pi;

    printf("pi:%f", pi);

    printf("sizeof(float):%u, sizeof(double):%u\n", sizeof(float), sizeof(double));

    int i = 0x01020304;
    double d = (double)i;
    printf("i %x %ld\n", *(long*)&i, i);
    printf("%x, d = %f\n", *(long*)&d, d);

    return 0;
}
