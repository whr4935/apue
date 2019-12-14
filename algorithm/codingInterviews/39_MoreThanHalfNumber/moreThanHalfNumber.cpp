/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

template <typename T>
inline void swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}

int partition(int data[], int start, int end)
{
    int v = data[end];
    int i, j;

    i = start-1;
    j = end;

    for (;;) {
        while (data[++i] < v);
        while (data[--j] > v);
        if (i >= j)
            break;
        swap(data[i], data[j]);
    }
    swap(data[i], data[end]);

    return i;
}

int moreThanHalfNum(int *numbers, int length)
{
    int start = 0;
    int end = length-1;
    int mid = length >> 1;
    int d = -1;

    while (start <= end) {
        d = partition(numbers, start, end);
        if (d == mid)
            break;

        if (d > mid) {
            end = d-1;
        } else {
            start = d+1;
        }
    }

    return numbers[d];
}

int moreThanHalfNumV2(int *numbers, int length)
{
    int result;
    int count = 0;

    for (int i = 0; i < length; ++i) {
        if (count == 0) {
            count = 1;
            result = numbers[i];
        } else if (result == numbers[i]) {
            ++count;
        } else {
            --count;
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    int data[] = {1, 2, 3, 2, 2, 2, 5, 4, 2};

    std::cout << moreThanHalfNumV2(data, sizeof(data)/sizeof(*data)) << std::endl;

    return 0;
}

