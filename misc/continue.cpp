#include <stdio.h>

int main(int argc, char *argv[])
{
    int cnt = 0;

    do {
        ++cnt;
        if (cnt > 1) {
            break;
        }
        continue;
        printf("hello\n");
    } while (1);
    printf("finished!\n");
    return 0;
}
