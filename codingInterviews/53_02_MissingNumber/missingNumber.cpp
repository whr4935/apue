/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

int getMissingNumber(const int* numbers, int length)
{
    int ret = -1;
    int start = 0;
    int end = length-1;
    int mid;

    while (start <= end) {
        mid = (start+end)>>1;

        if (numbers[mid] > mid) {
            if (mid==start || numbers[mid-1]==mid-1) {
                ret = mid;
                break;
            }
            end = mid-1;
        } else {
            start = mid+1;
        }
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int data[] = {0, 1, 2, 4, 5};
    
    std::cout << getMissingNumber(data, sizeof(data)/sizeof(*data)) << std::endl;

    return 0;
}
