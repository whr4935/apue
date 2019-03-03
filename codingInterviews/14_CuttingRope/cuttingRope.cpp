/*************************************************************************
 > File Name: cuttingRope.cpp
 > Created Time: 2018年04月09日 星期一 15时33分39秒
 ************************************************************************/

#include <iostream>
#include <algorithm>

int maxProductAfterCutting(int length)
{
    int *products = new int[length+1];
    int product;
    int max = 0;

    if (length == 1)
        return 1;
    else if (length == 2 || length == 3)
        return 2;

    products[0] = 0;
    products[1] = 1;
    products[2] = 2;
    products[3] = 3;

    for (int i = 4; i <= length; ++i) {
        for (int j = 1; j <= i/2; ++j) {
            product = products[j]*products[i-j];
            if (product > max)
                max = product;
        }
        products[i] = max;
    }

    delete [] products;

    return products[length];
}

int main(int argc, char** argv)
{
    int length = 8;

    std::cout << maxProductAfterCutting(length) << std::endl;

    return 0;
}

