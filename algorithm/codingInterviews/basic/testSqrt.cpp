/*************************************************************************
 > File Name: testSqrt.cpp
 > Created Time: 2018年04月09日 星期一 00时14分56秒
 ************************************************************************/

#include <iostream>
#include <Utilities/sqrt.h>
#include <math.h>


int main(int argc, char** argv)
{
    for (int i = 2; i <= 100; ++i) {
        double r = msqrt(i);
        std::cout << i << ": " << r << "(" << sqrt(i) << ")" << std::endl;
    }

    return 0;
}
