/*************************************************************************
 > File Name: reorderArray.cpp
 > Created Time: 2018年04月11日 星期三 15时53分19秒
 ************************************************************************/

#include <iostream>
#include <algorithm>

bool isEven(int d)
{
    return (d&1) == 0;
}

void reorder(int* data, int length, bool(*func)(int))
{
    int* beg, *end;

    beg = data-1;
    end = data + length;

    while (beg < end) {
        while (!func(*++beg)) ;
        while (func(*--end)) ;

        if (beg >= end)
            break;

        std::swap(*beg, *end);
    }
}

void reorderArray(int* data, int length)
{
    reorder(data, length, isEven);
}

int main(int argc, char** argv)
{
    int data[] = {1, 2, 3, 4, 5};
    int size = sizeof(data)/sizeof(*data);
    reorderArray(data, size);

    std::cout << "dump: ";
    std::for_each(std::begin(data), std::end(data), [](int d) {std::cout << d << " ";});
    std::cout << std::endl;

    return 0;
}

