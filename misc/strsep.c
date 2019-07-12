#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char str[] = {"abc,def"};
    char* pStr = str;

    char* p = 0;
    p = strsep(&pStr, ",");

    printf("pStr:%p, p:%p, %s\n", pStr, p, p);

    return 0;
}
