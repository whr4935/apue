#include <stdio.h>

int multiply5(int d)
{
    int ret = 0;
    __asm__ __volatile__(
            "lea (%1, %1, 4), %0;"
            :"=r"(ret)
            :"0"(d)
            );

    return ret;
}

int main()
{
    printf("multiply5(4)=%d\n", multiply5(4));
    return 0;

}
