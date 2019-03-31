#include <iostream>
#include <iomanip>

int main(int argc, char *argv[])
{
    int64_t a = 123243242354325;

    std::cout << std::fixed << a/1e6 <<std::endl;
    std::cout << 314.2 <<std::endl;

    return 0;
}
