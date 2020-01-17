#include <stdio.h>

extern char* g_array;

//the difference between array and pointer!
void testArray1()
{
    printf("%p\n", g_array); 
}

void testArray2(char* p)
{
    printf("%p\n", p);
}
