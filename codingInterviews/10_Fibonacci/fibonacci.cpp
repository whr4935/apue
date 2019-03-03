/*************************************************************************
 > File Name: fibonacci.cpp
 > Created Time: 2018年04月02日 星期一 10时33分09秒
 ************************************************************************/

#include <iostream>

long fibonacci(int n)
{
    long ret;
    long result[] = {0, 1, 2};

    if (n < 0) {
        throw std::logic_error("n < 0");
    }

    if (n >= 0 && n <= 2) {
        return result[n];
    }

    for (int i = 3; i <= n; ++i) {
        ret = result[2] + result[1];
        result[1] = result[2];
        result[2] = ret;
    }

    return ret;
}

int main(int argc, char** argv)
{
    std::cout << fibonacci(4) << std::endl;
    
    return 0;
}

