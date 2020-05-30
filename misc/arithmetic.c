#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    unsigned long long pts;

    pts = 0x12345678;

    pts |= 1LL << 32;

    printf("pts = %#llx\n", pts);

    unsigned int v = 0xfffffffe;
    int d = abs(v);

    printf("d = %d\n", d);

    return 0;
}
