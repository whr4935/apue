/*************************************************************************
 > File Name: mergeSort.cpp
 > Created Time: 2018年04月03日 星期二 21时28分59秒
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <time.h>


void mergeSortCore(int data[], int out[], int l, int r)
{
    if (l >= r)
        return;

    int m = (l + r) >> 1;

    mergeSortCore(data, out, l, m);
    mergeSortCore(data, out, m+1, r);

    int i, j;
    for (i = m; i >= l; --i) out[i] = data[i];
    for (j = m+1; j <= r; ++j) out[j] = data[r-j+m+1];
    ++i; --j;

    for (int k = l; k <= r; ++k) {
        if (out[i] < out[j]) {
            data[k] = out[i++];
        } else {
            data[k] = out[j--];
        }
    }
}

void mergeSort(int data[], int length)
{
    int *out = new int[length];
    memset(out, 0, length*sizeof(int));
    mergeSortCore(data, out, 0, length-1);

    delete [] out;
}

int main(int argc, char** argv)
{
    //int data[] = {2, 8, 9, 7, 4, 6};

    int *data;
    const int size = 1024 * 1024 * 20;
    data = new int[size];
    srand(time(NULL));
    for (size_t i = 0; i < size; ++i) {
        data[i] = rand();
    }
    
    //for (size_t i = 0; i < 10; ++i) {std::cout << data[i] << " "; };
    //std::cout << std::endl;

    std::cout << "mergeSort:";
    auto begin = std::chrono::high_resolution_clock::now();
    mergeSort(data, size);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms" << std::endl;
    //std::for_each(std::begin(data), std::end(data), [](int d) {std::cout << d << " ";});

    delete [] data;

    return 0;
}

