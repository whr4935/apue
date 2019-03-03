/*************************************************************************
 > File Name: power.cpp
 > Created Time: 2018年04月09日 星期一 17时09分55秒
 ************************************************************************/

#include <iostream>

double power(double base, int exponent)
{
    if (exponent == 0) 
        return 1.0;

    bool negative = false; 
    if (exponent < 0) {
        negative = true;
        exponent = -exponent;
    }

    double result = base;
    for (int i = 1; i < exponent; ++i) {
        result *= base;
    }

    if (negative) {
        result = 1.0 / result;
    }

    return result;
}

int main(int argc, char** argv)
{
    std::cout << power(2, -0) << std::endl;    
    return 0;
}

