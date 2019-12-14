/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <cmath>

int digitsInSequence(int k)
{
    int n = 0;
    int width = 1;

    for (;;) {
        if (width > 1)
            n = std::pow(10, width) - std::pow(10, width-1);
        else
            n = 10;
        n *= width;
        if (n >= k)
            break;
        k -= n;
        ++width;
    }

    int d = k / width;
    int r = k % width;
    n = 0;
    if (width > 1)
        n = std::pow(10, width-1);
    n += d;
    while (r != width) {
        d = n % 10;
        n /= 10;
        ++r;
    }

    return d;
}

int main(int argc, char *argv[])
{
    std::cout << digitsInSequence(1001) << std::endl;
    return 0;
}
