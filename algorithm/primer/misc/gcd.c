#include <includes.h>


int gcd(int u, int v)
{
    int t;

    while (u > 0) {
        if (u < v) {
            t = u;
            u = v;
            v = t;
        }
        u = u - v;
    }

    return v;
}

int gcd_v2(u, v)
{
    int t;

    t = u % v;
    if (t) {
        return gcd_v2(v, t);
    }

    return v;
}



int test_gcd()
{
    int ret = 0;
    int ret_v2 = 0;

    int data1 = 6;
    int data2 =-4 ;


    ret = gcd(data1, data2);
    ret_v2 = gcd_v2(data1, data2);
    assert(ret == ret_v2);

    printf("gcd ret = %d\n", ret);


    return 0;
}
