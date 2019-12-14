/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

int greatestSum(int data[], int length)
{
    int sum = 0;
    int great = 0;

    for (int i = 0; i < length; ++i) {
        if (sum <= 0) {
            sum = data[i];
        } else {
            sum += data[i];
        }

        if (sum > great) {
            great = sum;
        }
    }

    return great;
}

int main(int argc, char *argv[])
{
    int data[] = {1, 3, -8, 2, -1, 4, -4};
    int total = greatestSum(data, sizeof(data)/sizeof(*data));
    std::cout << total << std::endl;
    
    return 0;
}
