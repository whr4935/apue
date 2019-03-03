/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>
#include <sstream>

inline int powerOfLen(int len)
{
    int ret = 1;
    for (int i = 0; i < len; ++i) {
        ret *= 10;
    }

    return ret;
}

int numberOf1(const char* strN, int len)
{
    int first = strN[0] - '0';
    int numFirst = 0;
    int numOther = 0;

    if (len <= 1) {
        return *strN != '0';
    }

    if (first == 1) {
        numFirst = atoi(strN+1)+1;
    } else if (first > 1) {
        numFirst = powerOfLen(len-1);
    }

    numOther = first * powerOfLen(len-2) * (len-1); // ?

    return numFirst + numOther + numberOf1(strN+1, len-1);
}

int numberOf1Between1AndN(int n)
{
    if (n <= 0)
        return 0;

    std::stringstream str;
    str << n;
    return numberOf1(str.str().c_str(), str.str().size());
}

inline int numberOf1Digit(int n)
{
    int ret = 0;

    while (n) {
        if (n%10 == 1)
            ++ret;
        n /= 10;
    }

    return ret;
}

int numberOf1Between1AndN_V2(int n)
{
    int ret = 0;
    for (int i = 0; i <= n; ++i) {
        ret += numberOf1Digit(i); 
    }
    return ret;
}

int numberOf1Between1AndN_V3(int n)
{
    int ans = 0;
    int exp = 1;

    while (n / exp) {
        ans += n / (exp * 10) * exp;
        if (n % (exp * 10) / exp > 1)
            ans += exp;
        else if (n % (exp * 10) / exp == 1)
            ans += (n % exp + 1);

        exp *= 10;
    }

    return ans;
}

int main(int argc, char *argv[])
{
    std::cout << numberOf1Between1AndN(12) << std::endl; 
    std::cout << numberOf1Between1AndN(21345) << std::endl; 

    std::cout << numberOf1Between1AndN_V2(12) << std::endl; 
    std::cout << numberOf1Between1AndN_V2(21345) << std::endl; 

    std::cout << numberOf1Between1AndN_V3(12) << std::endl; 
    std::cout << numberOf1Between1AndN_V3(21345) << std::endl; 

    return 0;
}
