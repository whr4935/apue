#include <stdio.h>

#define ATOMIC_READ(x) __sync_fetch_and_or(x, (typeof(x))0)

int main(int argc, char *argv[])
{
    int b = 3;
    int c = ATOMIC_READ(&b);
    printf("c = %d\n", c);

#if A
    printf("define!!! %d \n", A);
#endif

#ifdef B
    printf("define B:%s\n", B);
#else
    printf("B undefined!\n");
#endif

    return 0;
}
