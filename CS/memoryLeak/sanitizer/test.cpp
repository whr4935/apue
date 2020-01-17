#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char* p = (char*)malloc(10);

    printf("p:%p\n\n", p);

    return 0;
}
