#include <stdio.h>

int main(int argc, char *argv[])
{
    float a = -2.0;
   int i = (int)a; 
   unsigned int ui = (unsigned int)a;
   printf("i:%d, ui:%u (%#x)", i, ui, ui);
    return 0;
}
