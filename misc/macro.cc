#include <stdio.h>
int g_debug  = 1;

#define PRINT(fmt,...)   ({g_debug ? printf(fmt "\n", ##__VA_ARGS__) : 0;})

int main(int argc, char *argv[])
{
    PRINT("hello world!");

    return 0;
}
