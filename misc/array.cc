#include <iostream>

int main(int argc, char *argv[])
{
    typedef char aa[1]; 
    aa a;

    int a1[2][3] {{1, 2, 3}, {4, 5, 6}};

    int (*p)[3] = &a1[1];

    printf("%d", p[0][0]);

    return 0;
}
