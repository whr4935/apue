/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <algorithm>
#include <cassert>

int inversePairsCore(int *data, int* buf, int start, int end)
{
    int count = 0;
    int mid = (end+start)>>1;
    int p1 = mid;
    int p2 = end;
    int p3 = end;

    if (start >= end)
        return 0;

    count += inversePairsCore(data, buf, start, mid);
    count += inversePairsCore(data, buf, mid+1, end);

    while (p1 >= start && p2 > mid) {
        if (data[p1] > data[p2]) {
            count += p2-mid;
            buf[p3--] = data[p1];
            --p1;
        } else {
            buf[p3--] = data[p2];
            --p2;
        }
    }
    while (p1 >= start) {
        buf[p3--] = data[p1--];
    }
    while (p2 > mid) {
        buf[p3--] = data[p2--];
    }
    assert(p3==start-1);

    ++p3;
    while (p3 <= end) {
        data[p3] = buf[p3];
        ++p3;
    }

    return count;
}

int inversePairs(int *data, int length)
{
    int count = 0;
    int *buf = new int[length];

    count = inversePairsCore(data, buf, 0, length-1);
    
    delete [] buf;
    return count;
}

int main(int argc, char *argv[])
{
    int data[] = {7, 5, 6, 4};
    std::cout << inversePairs(data, sizeof(data)/sizeof(*data)) << std::endl;
    
    return 0;
}
