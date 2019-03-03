/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <cstring>

int getMaxValue(const int* values, int m, int n)
{
    int maxValue = 0;
    int value;
    int *v = new int[n];
    int v1, v2;
    memset(v, -1, n);

    int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            value = values[m*i+j];
            v1 = j > 0 ? v[j-1] : 0;
            v2 = i > 0 ? v[j] : 0;
            v[j] = std::max(v1, v2) + value;
        }
    }
    maxValue = v[n-1];

    delete [] v;
    return maxValue;
}

int main(int argc, char *argv[])
{
    int values[][4] = {
    {1, 10, 3, 8},
    {12, 2, 9, 6},
    {5, 7, 4, 11},
    {3, 7, 16, 5}
    }; 

    std::cout << getMaxValue((int*)values, 4, 4) << std::endl;

    return 0;
}
