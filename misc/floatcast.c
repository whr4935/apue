#include <stdio.h>

int main(int argc, char *argv[])
{
    float a = -2.0;
   int i = (int)a; 
   unsigned int ui = (unsigned int)a;
   printf("i:%d, ui:%u (%#x)\n", i, ui, ui);

   int a1 = 1.0;
   float f1 = 1.0;

   printf("f1:%x\n", *(int*)&f1);

   if (a1 == f1) {
       printf("yes\n");
   }

   signed char a2 = 1;
   unsigned char b2 = -127;
   signed char c2 = a2 - b2;
   if (a2 > b2) {
       printf("hello\n");
   }
   printf("%d, %d, %u\n", c2, a2 > b2, b2);
   printf("%u\n",  (unsigned char)b2);

   unsigned char a3 = 1;
   unsigned char b3 = 254;
   unsigned char c3 = a3 - b3;
   printf("c3 = %d\n", c3);

    return 0;
}
