/*************************************************************************
 > File Name: minNumberInRotatedArray.cpp
 > Created Time: 2018年04月08日 星期日 17时53分48秒
 ************************************************************************/

#include <iostream>
#include <exception>

int sequenceFindMinNumber(int data[], int length)
{
    int min = data[0];
    for (int i = 1; i < length; ++i) {
        if (data[i] < min) {
            min = data[i];
        } 
    }

    return min;
}

int minNumberInRotatedArray(int data[], int length)
{
    if (data == nullptr || length <= 0)
        throw std::logic_error("arguments error!");

    int *p1 = data, *p2 = data + length - 1;
    int *mid = data;

    //while (*p1 >= *p2) {
    for (;;) {
        if (p2-p1==1) {
            return *p2;
        }
        
        mid = p1 + ((p2-p1)>>1);
        if (*p1 == *p2 && *mid == *p1) {
            std::cout << "do sequence find" << std::endl;
            return sequenceFindMinNumber(data, length);
        }

        if (*mid >= *p1)
            p1 = mid;
        else if (*mid <= *p2)
            p2 = mid;
        else
            throw std::logic_error("logic error!");
    }

    return *mid;
}

int main(int argc, char** argv)
{
    int data[] = {1, 1, 1, 0, 3};

    std::cout << "min: " << minNumberInRotatedArray(data, sizeof(data)/sizeof(*data)) << std::endl;

    return 0;
}
