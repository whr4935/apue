#include <stdio.h>

int main(int argc, char *argv[])
{
    int d = (0XFF << 2) & 0xFF; 
    printf("d = %#x", d);
    return 0;
}
