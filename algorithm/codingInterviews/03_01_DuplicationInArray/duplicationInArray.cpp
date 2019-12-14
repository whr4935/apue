/*************************************************************************
 > File Name: 03_01_DuplicationInArray/duplicationInArray.cpp
 > Created Time: 2018年03月27日 星期二 20时31分36秒
 ************************************************************************/

#include <iostream>

bool duplicate(int numbers[], int length, int* duplication)
{
    int i;
    int d;

    *duplication = -1;

    for (i = 0; i < length; ++i) {
        while (numbers[i] != i) {
            d = numbers[i];
            if (d == numbers[d]) {
                *duplication = d;
                return true;
            }
            numbers[i] = numbers[d];
            numbers[d] = d;
        }
    }
    
    return false;
}


int main()
{
    int numbers[] = {4, 0, 6, 1, 2, 3, 5};
    int duplication = -1;
    if (duplicate(numbers, sizeof(numbers)/sizeof(*numbers),  &duplication)) {
        std::cout << "duplication: " << duplication << std::endl;
    } else {
        std::cout << "no duplication!" << std::endl;
    }

    return 0;
}
