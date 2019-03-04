#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    printf("sizeof long:%lu\n", sizeof(long)); 
    printf("sizeof long:%lu\n", sizeof(uintptr_t)); 

    return 0;
}
