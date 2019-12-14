/*************************************************************************
 > File Name: 03_02_DuplicationInArrayNoEdit/duplicationInArray.cpp
 > Created Time: 2018年03月27日 星期二 20时58分01秒
 ************************************************************************/
#include <iostream>

int countRange(const int* numbers, int length, int start, int end)
{
    int count = 0;

    for (int i = 0; i < length; ++i) {
        if (numbers[i]>=start && numbers[i] <= end)
            ++count;
    }

    return count;
}

int getDuplication(const int* numbers, int length)
{
    int start = 1;
    int end = length-1;
    int mid;
    int range;
    int count;
    bool flag = true;

    while (start <= end) {
        if (flag)
            mid = start + ((end-start)>>1);
        count = countRange(numbers, length, start, mid);
        if (end == start) {
            if (count > 1)
                return start;
            else
                break;
        }

        flag = true;
        range = mid - start + 1;
        if (count > range) {
            end = mid;
        } else if (count == range) {
            if (mid == start) {
                start = mid+1;
            } else {
                mid -= 1;
                flag = false;
            }
        } else {
            start = mid+1;
        }
    }
    
    return -1;
}  

int main()
{
    int numbers[] = {3, 6, 5, 1, 4, 2, 7, 7};
    int duplication = getDuplication(numbers, sizeof(numbers)/sizeof(*numbers));
    if (duplication >= 0) {
        std::cout << "duplication:" << duplication << std::endl;
    } else {
        std::cout << "no duplication!" << std::endl;
    }

    return 0;
}
