#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned long long pts;

    pts = 0x12345678;

    pts |= 1LL << 32;

    printf("pts = %#llx\n", pts);
    return 0;
}
