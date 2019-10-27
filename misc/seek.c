#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        return 0;
    }
    
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("error !\n");
    }

    fseek(fp,  0, SEEK_END);

    long pos = ftell(fp);

    printf("size is %ld\n", pos);

    return 0;
}
