#include <stdio.h>
int MyGlobalVar;

extern void init();

void  test_routine()
{
    printf("main test_routine!\n");
}

int main() {
    init();

    printf("MyGlobalVar = %d, MyGlobalVar addr = %p\n ",MyGlobalVar, &MyGlobalVar);

}
