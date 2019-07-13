#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 10;

    __atomic_store_n(&a, 20, __ATOMIC_RELEASE); 

    int b = __atomic_load_n(&a, __ATOMIC_ACQUIRE);

    
    printf("a = %d, b = %d\n",a, b);

    return 0;
}
