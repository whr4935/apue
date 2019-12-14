/*************************************************************************
 > File Name: sqrt.cpp
 > Created Time: 2018年04月09日 星期一 00时07分50秒
 ************************************************************************/

#include "sqrt.h"

template <typename T>
T abs(T x)
{
    return x < static_cast<T>(0) ? -x : x;
}

#define Epsilon 1e-10

double msqrt(double x)
{
    double r0;
    double r = 1.0;
    do {
        r0 = r;
        r = (x-1)/(r+1) + 1;
    } while (abs(r - r0) > Epsilon);

    return r;
}


