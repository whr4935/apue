#include"algorithm.h"

int gcd_v1(int n1, int n2)
{
    int r;
    if (n1 > n2) {
        r = n1 % n2;
        if (r == 0)
            return n2;
        else {
            return gcd_v1(n2, r);
        }
    } else {
        r = n2 % n1;
        if (r == 0)
            return n1;
        else {
            return gcd_v1(n1, r);
        }
    }
}

int gcd_v2(int m, int n)
{
    int tmp;

    while (m) {
        //m is remainder, until it less than divisor
        if (m < n) {
            tmp = m;
            m = n;
            n = tmp;
        }

        m -= n;
    }

    return n;
}

int gcd_v3(int m, int n)
{
    int tmp;
    int r;

    if (m < n) {
        tmp = m;
        m = n;
        n = tmp;
    }

    r =  m % n;
    return r != 0 ? gcd_v3(n, r) : n;
}

int TEST_gcd()
{
    struct Func {
        int (*pf)(int, int);
        char *name;
    } f[] =  {
        /* {&gcd_v1, "gcd_v1"}, */
        {&gcd_v2, "gcd_v2"},
        {&gcd_v3, "gcd_v3"},

    };

    struct Data {
        int m;
        int n;
    };

    struct Data d[] = {
        {40, 20},
        {3, 2},
        {4, 6},
        {9, 5},
        {5, 9},
        {2, 7},
        {461952, 116298},


    };

    int i;
    int j;
    int r;

    for (j = 0; j < ARRAY_SIZE(f); ++j) {
        for (i = 0; i < ARRAY_SIZE(d); ++i) {
            r = f[j].pf(d[i].m, d[i].n);
            printf("%s(%d, %d) = %d\n", f[j].name, d[i].m, d[i].n, r);
        }
    }

    printf("2 % 5 = %d\n", 2 % 5);
    printf("5 % 2 = %d\n", 5 % 2);

    return 0;
}
