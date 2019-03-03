#include <stdio.h>

namespace myname {
int var = 42;
}

int g_var = 48;

//extern int _ZN6myname3varE;

extern char _edata[];

int main(int argc, char *argv[])
{
    printf("%d\n", _ZN6myname3varE);
    printf("%p\n", _edata);
    return 0;
}
