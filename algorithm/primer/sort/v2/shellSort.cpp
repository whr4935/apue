/*************************************************************************
 > File Name: shellSort.cpp
 > Created Time: 2018年04月04日 星期三 15时44分49秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>

void shellSort(int data[], int length)
{
    int i, j, tmp;
    int step = 1;
    for (; step < length/9; step=step*3+1) ;

    //uncomment the follow line -> insertSort, it's very slow.
    //step = 1;

    while (step) {
        for (i = step; i < length; ++i) {
            j = i;
            tmp = data[j];
            while (j >= step && tmp < data[j - step]) {
                data[j] = data[j-step];
                j -= step;
            }
            data[j] = tmp;
        }
        step /= 3;
    }
}

#define TEST_BIG_DATA   1

int main(int argc, char** argv)
{

#if !TEST_BIG_DATA
    int data[] = {2, 9, 7, 26, 5};
    const int size = sizeof(data)/sizeof(*data);
#else
    std::default_random_engine gen;
    const int size = 1024*1024*10;
    int *data = new int[size];
    for (int i = 0; i < size; ++i) {
        data[i] = gen();
    }
#endif

#if TEST_BIG_DATA
    auto begin = std::chrono::high_resolution_clock::now();
#endif

    shellSort(data, size);

#if TEST_BIG_DATA
    auto end = std::chrono::high_resolution_clock::now();
#endif
    std::cout << "shellSort: ";

#if !TEST_BIG_DATA
    std::for_each(std::begin(data), std::end(data), [](int d){std::cout << d << " ";});
#else
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms" << std::endl;
    delete [] data;
#endif


    return 0;
}
