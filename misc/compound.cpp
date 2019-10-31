#include <stdio.h>
#include <iostream>

int& f1(int& a)
{
    std::cout << __FUNCTION__ << std::endl;
    return a;
}

int f2(int b)
{
    std::cout << __FUNCTION__ <<  std::endl;
    return b;
}

int f3(int c)
{
    std::cout << __FUNCTION__ <<  std::endl;
    return c;
}

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    int c = 3;

    f1(a) += f2(b) + f3(c);

    return 0;
}
