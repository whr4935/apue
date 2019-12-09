#include <stdio.h>

void foo(int* p)
{
    {
        int *pp = p;

    }

    int*& rp = p;

    *rp = 4;

    printf("rp = %d\n", *rp);

    printf("rp:%p, p:%p, a:%p\n", &rp, &p, p);
}

int main(int argc, char *argv[])
{
    int a = 3;
    int &ra = a;

    printf("&ra:%p, &a:%p\n", &ra, &a);

    foo(&a);

    printf("a = %d\n", a);

    return 0;
}
