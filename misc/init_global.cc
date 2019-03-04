#include <stdio.h>
#include <time.h>

time_t g_t = time(NULL);

int test()
{
    /*return g_v1;*/
    return 0;
}

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

    return 0;
}
