#include <iostream>
#include <iomanip>

int main(int argc, char *argv[])
{
    std::cout << std::hex <<std::showbase;
    std::cout << std::boolalpha;
    std::cout << std::uppercase;

    std::cout << 16 <<std::endl;

    std::cout << std::setprecision(2);
    std::cout <<std::scientific;
    std::cout << std::right;
    std::cout << 4.1343434 << std::endl;

    bool a = false;

    std::cout <<a;

    return 0;
}
