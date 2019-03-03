/*************************************************************************
 > File Name: traits.cpp
 > Created Time: 2018年03月28日 星期三 13时27分53秒
 ************************************************************************/

#include<iostream>

template <typename T>
struct is_void {
    static const bool value = false;
};

template <>
struct is_void<void> {
    static const bool value = true;
};


int main(int argc, char** argv)
{
    std::cout << std::boolalpha;
    std::cout << is_void<int>::value << std::endl;
    std::cout << is_void<void>::value << std::endl;
    
    return 0;
}

