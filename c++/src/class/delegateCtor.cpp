#include <iostream>


class A {
public:
    A(int a, int b, int c)
        : m_a(a), m_b(b), m_c(c)
        //: A(a), m_b(b), m_c(c) //this is an error!!!
    {

    }

    A(int a)
        : A(a, 0, 0) {}

private:
    int m_a;
    int m_b;
    int m_c;
};


void testDelegateCtor()
{
    A a1(1, 2, 3);
    A a2(1);
}
