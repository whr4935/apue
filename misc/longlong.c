#include <stdio.h>

int main(int argc, char *argv[])
{
#ifdef __LP64__
    printf("%lu %lu", sizeof(long), sizeof(long long));
#endif
    return 0;
}
