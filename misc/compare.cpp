#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = -1;
    unsigned int b = 2;

    if (a < b) {
        printf("little\n");
    } else {
        printf("big\n");
    }

    return 0;
}
