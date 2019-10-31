#include <stdio.h>

int main(int argc, char *argv[])
{
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
