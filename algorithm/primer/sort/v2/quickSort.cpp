/*************************************************************************
 > File Name: quickSort.cpp
 > Created Time: 2018年04月02日 星期一 21时30分11秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <chrono>
#include <time.h>

template <typename T>
inline void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

void prePartition(int data[], int start, int mid, int end)
{
    if (data[start] > data[mid])
        swap(data[start], data[mid]);
    if (data[mid] > data[end])
        swap(data[mid], data[end]);
    if (data[start] > data[mid])
        swap(data[start], data[mid]);
}

int partition(int data[], int start, int end)
{
    int mid = (start+end)>>1;
    prePartition(data, start, mid, end);
    swap(data[mid], data[end-1]);
    ++start;
    --end;

    int part = data[end];
    int i = start-1;
    int j = end;

    for (;;) {
        while (data[++i] < part) ;
        while (data[--j] > part) ;
        if (i >= j)
            break;
        swap(data[i], data[j]);
    }
    swap(data[i], data[end]);
    
    return i;  
}

void quickSortCore(int data[], int start, int end)
{
    if (end-start<=1) {
        if (end>start && data[start]>data[end])
            swap(data[start], data[end]);
        return;
    }

    int idx = partition(data, start, end);
    quickSortCore(data, start, idx-1);
    quickSortCore(data, idx+1, end);    
}

void quickSort(int data[], int length)
{
    quickSortCore(data, 0, length-1);
}

int main(int argc, char** argv)
{
#if 0
    int data[] = {2, 5, 3, 8, 6, 1};
    quickSort(data, sizeof(data)/sizeof(*data));
    std::cout << "quickSort:";
    std::for_each(std::begin(data), std::end(data), [](int d) {
            std::cout << d << " ";
            });
    std::cout << std::endl;
#else
    const int size = 1024*1024*20 ;
    int *data = new int[size];
    
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        data[i] = rand();
    }

    std::cout << "quickSort:";
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    quickSort(data, size);
    auto end = std::chrono::high_resolution_clock::now();
    auto result = end - begin;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(result).count() << " ms" << std::endl;


    delete [] data;
#endif

    return 0;
}

