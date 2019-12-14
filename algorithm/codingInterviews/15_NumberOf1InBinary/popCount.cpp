/*************************************************************************
 > File Name: popCount.cpp
 > Created Time: 2018年04月09日 星期一 16时23分29秒
 ************************************************************************/

#include <iostream>

int popCount(int num)
{
    int count = 0;

    while (num) {
        num = num & (num-1);
        ++count;
    }

    return count;
}

int main(int argc, char** argv)
{
    std::cout << popCount(0xF0FFFFFF) << std::endl; 
    return 0;
}

