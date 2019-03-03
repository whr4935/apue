/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

int integerIdenticalToIndex(int* data, int length)
{
    int start = 0;
    int end = length-1;
    int mid;
    int ret = -1;

    while (start <= end) {
        mid = (start+end)>>1;
        if (mid==data[mid]) {
            ret = mid;
            break;
        } else if(data[mid]>mid) {
            end = mid-1;
        } else {
            start = mid+1;
        }
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int data[] = {-3, -1, 1, 3, 5};
    std::cout << integerIdenticalToIndex(data, sizeof(data)/sizeof(*data)) << std::endl;

    return 0;
}
