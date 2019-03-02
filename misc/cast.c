#include <stdio.h>

void test(const char*s)
{
    puts(s);
}

int main(int argc, char *argv[])
{
    short a = -1;
    unsigned short b = 65535;

    int a1 = a;
    int b1 = b;
    printf("%d, %d\n", a1, b1);

    test(__FUNCTION__);

    return 0;
}
