#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 2;
    if (a & 1<<2) {
        printf("yes\n");
        return 0;
    }


    FILE* fp = fopen("a", "r");
    if (fp == NULL) 
        return -1;

    int size = fseek(fp, -1, SEEK_END);

    size = ftell(fp);
    printf("size:%d\n", size);

    fclose(fp);

    return 0;
}
