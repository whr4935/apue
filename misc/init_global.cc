#include <stdio.h>
#include <time.h>

time_t g_t = time(NULL);

int test()
{
    /*return g_v1;*/
    return 0;
}

enum ABC {
    AA,
    BB,
};

struct init_struct {
    int a;
    int b;
    ABC abc;
};

/*int g_v1 = 1;*/

int main(int argc, char *argv[])
{
    /*printf("g_v1 = %d\n", g_v1);*/
    /*printf("g_t = %d\n", g_t);*/

    unsigned short a = 65535;
    unsigned short b = 0;
    unsigned short one = 1;
    /*if (((a+1)&65536-1)==b) {*/
    if ((b-a&65535)==1) {
        printf("equal\n");
    } else {
        printf("not equal\n");
    }

    int array_v[10]{1, 2, 3};
    int array_v2[10];

    init_struct init_s{}, init_s2;

    init_struct init_s3;

    printf("init_s3.b = %d, v2:%d, v1:%d\n", init_s3.b, array_v2[2], array_v[1]);

    return 0;
}
