#include <iostream>

int main(int argc, char *argv[])
{
    int a = -1;
    int b;
    const char* s = "123 456";
    sscanf(s, "%*d %d", &a, &b);

    std::cout << a << std::endl;

    return 0;
}
