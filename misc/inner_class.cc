#include <iostream>

struct A{
    int m_a = 1;
    struct InnerA {
        int mInnerA = 2;
        InnerA() {
            //mInnerA = m_a;
        }
    };
};

int main(int argc, char *argv[])
{
    std::cout <<"aaa inner" <<std::endl;    
    return 0;
}
