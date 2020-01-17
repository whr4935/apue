#include <stdio.h>

char g_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

extern void testArray1();
extern void testArray2(char* p);

int main(int argc, char *argv[])
{
    testArray1();
    testArray2(g_array);

    return 0;
}
