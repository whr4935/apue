#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned char a = 1;
    char b = -1;

   a -= b;
    printf("a = %d\n", a);
    return 0;
}
