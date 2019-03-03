/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

void downHeap(int *p, int k, int m)
{
    int v = p[m];
    int j;
    while (m<(k-1)/2) {
        j = m + m + 1;
        if (j < k-1 && p[j] < p[j+1]) ++j;
        if (v >= p[j]) break;
        p[m] = p[j]; m = j;
    }
    p[m] = v;
}

void getLeaseNumbers(int* input, int n, int* output, int k)
{
    for (int i = 0; i < n; ++i) {
        if (i < k) {
            output[i] = input[i];
            continue;
        } else if (i == k) {
            for (int j = k/2-1; j >= 0; --j) {
                downHeap(output, k, j);
            }
        }
        output[0] = input[i];
        downHeap(output, k, 0);
    }
}

int main(int argc, char *argv[])
{

    int data[] = {9, 20, 3, 7, 9, 8, 5, 4, 2};
    int output[3] = {-1};

    getLeaseNumbers(data, sizeof(data)/sizeof(*data), output, sizeof(output)/sizeof(*output));

    std::cout << "least k numbers:";
    for (unsigned int i = 0; i < sizeof(output)/sizeof(*output); ++i) {
        std::cout << output[i] << " ";
    }
    std::cout << std::endl;


    return 0;
}
