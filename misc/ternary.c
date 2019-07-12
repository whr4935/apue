#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 4;

    //求值顺序和结合性是两个不同的东西
    int b = a > 2 ? 2 : (a > 1) ? 1 : 0;

    printf("b = %d\n", b);

    return 0;
}
