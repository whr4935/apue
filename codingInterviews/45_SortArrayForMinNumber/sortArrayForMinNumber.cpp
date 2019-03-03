/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>
#include <algorithm>

template<unsigned int N>
std::string sortArrayForMinNumber(int (&data)[N])
{
    std::string ret;
    
    std::sort(std::begin(data), std::end(data), [](const int&a, const int&b) {
                std::string s1 = std::to_string(a) + std::to_string(b);
                std::string s2 = std::to_string(b) + std::to_string(a);
                return s1 < s2;
            });

    for (auto d : data) {
        ret += std::to_string(d);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int data[] = {3, 32, 321};    
    std::cout << sortArrayForMinNumber(data)<< std::endl;

    return 0;
}
