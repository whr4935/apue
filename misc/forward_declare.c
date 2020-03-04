#include <stdio.h>

struct object_t;


struct Base {
    int a;
    int b;
};

struct Base g_b[2] = {
    {123, 0}
};

int main(int argc, char *argv[])
{
    struct object_t* p = NULL;

    printf("p = %p\n", p);


    for (size_t i = 0; i < 2; ++i) {
        printf("%zu: a = %d, b = %d\n", i, g_b[i].a, g_b[i].b);
    }

    return 0;
}


