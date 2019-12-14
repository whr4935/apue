/*************************************************************************
 > File Name: basic/basic.cpp
 > Created Time: 2018年03月26日 星期一 23时08分01秒
 ************************************************************************/

#include <iostream>

class Base
{
public:
    Base() {
        std::cout << "ctor" << std::endl;
    }

    ~Base() {
        std::cout << "dtor" << std::endl;
    }

    void hello() {
        std::cout << "hell world!" << std::endl;
    }
};


int main()
{
    {
        Base b;
        b.hello();
    }

    return 0;
}
