#include <stdio.h>

int x = 15213;
int y = 15213;

void f();

int a;

int main(int argc, char *argv[])
{
    static int b;
    printf("a:%d, b:%d\n", a, b);

    f();
    printf("x = %d, y = %d\n", x, y);  
    return 0;
}
