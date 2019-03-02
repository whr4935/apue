#include <stdio.h>

int main(int argc, char *argv[])
{
    double pi = 3.14;
    int pi2 = (int)pi;

    printf("pi:%f", pi);

    printf("sizeof(float):%u, sizeof(double):%u\n", sizeof(float), sizeof(double));

    return 0;
}
