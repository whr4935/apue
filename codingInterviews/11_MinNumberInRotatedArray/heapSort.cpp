/*************************************************************************
 > File Name: heapSort.cpp
 > Created Time: 2018年04月04日 星期三 10时16分52秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

// 2*k+1 2*k+2
void downHeap(int data[], int length, int k)
{
    int t;
    int d = data[k];

    while (k < (length-1)/2) {
        t = k + k + 1;
        if (t < length-1 && data[t+1] > data[t]) ++t;
        if (d > data[t])
            break;
        data[k] = data[t];
        k = t;
    }
    data[k] = d;
}

void heapSort(int data[], int length)
{
    for (int i = (length-1)/2; i >= 0; --i) {
        downHeap(data, length, i);
    }

    int tmp;
    for (int i = length-1; i >= 0; --i) {
        tmp = data[0];
        data[0] = data[i];
        data[i] = tmp;
        downHeap(data, i, 0);
    }
}

int main(int argc, char** argv)
{
    std::default_random_engine gen;
    const int size = 1024*1024 * 20;
    int *data = new int[size];
    for (int i = 0; i < size; ++i) {
        data[i] = gen();
    }

    auto begin = std::chrono::high_resolution_clock::now();
    heapSort(data, size);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() <<" ms" << std::endl;

    //std::for_each(std::begin(data), std::end(data), [](int d) {std::cout << d << " ";});
    //std::cout << std::endl;

    return 0;
}
