/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

template <typename T>
T min3(const T& a, const T& b, const T& c)
{
    return a < b ? (a < c ? a : c) : b < c ? b : c;
}

int getUglyNumber(int index)
{
    int ugly = -1;
    int *uglyNumbers = new int[index];
    int* pMultiply2 = uglyNumbers;
    int* pMultiply3 = uglyNumbers;
    int* pMultiply5 = uglyNumbers;
    int uglyIndex = 0;

    uglyNumbers[uglyIndex++] = 1;

    while (uglyIndex < index) {
        while (*pMultiply2*2 <= uglyNumbers[uglyIndex-1])
            ++pMultiply2;
        while (*pMultiply3*3 <= uglyNumbers[uglyIndex-1])
            ++pMultiply3;
        while (*pMultiply5*5 <= uglyNumbers[uglyIndex-1])
            ++pMultiply5;

        ugly = min3(*pMultiply2*2, *pMultiply3*3, *pMultiply5*5);
        uglyNumbers[uglyIndex++] = ugly;
    }

    ugly = uglyNumbers[index-1];
    delete [] uglyNumbers;
    return ugly;
}

bool isUgly(int number)
{
    while (number%2 == 0) number /= 2;
    while (number%3 == 0) number /= 3;
    while (number%5 == 0) number /= 5;

    return number == 1;
}

int getUglyNumber_V2(int index)
{
    int count = 0;
    int i = 0;
    while (count < index) {
        ++i;
        if (isUgly(i))
            ++count;
    }

    return i;
}

int main(int argc, char *argv[])
{
    std::cout <<    getUglyNumber(1500) << std::endl;
    std::cout << getUglyNumber_V2(1500) << std::endl;

    return 0;
}
