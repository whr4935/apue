#include <iostream>
#include <stdint.h>

uint32_t roundUpTwo(uint32_t d)
{
    --d;

    d |= d >> 1;
    d |= d >> 2;
    d |= d >> 4;
    d |= d >> 8;
    d |= d >> 16;

    ++d;
    return d;
}


int main(int argc, char *argv[])
{
    std::cout << roundUpTwo(1103) <<std::endl;
    return 0;
}
