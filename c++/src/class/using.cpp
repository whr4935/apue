#include <iostream>

struct A1 {
    typedef int A2int;
    struct A2 {
        static void fun() {
            std::cout << "A1::A2::fun" << std::endl;
        }
    };
};

template<typename T>
struct A {
    //typedef T Type;
    using Type = T;

    int ma = 11;
};

template<typename T>
using Aint = A<T>;

//template<typename T>
//typedef A<T> Aint_t;

template<typename T>
struct B : A<T>
{
    using typename A<T>::Type;
};


struct Base
{
    Base(int a) 
        : mData(a)
    {
        std::cout << "Base ctor" << std::endl;
    }

    int mData;
};

struct Derived : Base
{
    using Base::Base;

    int mD2 = 20;
};

struct C1
{
    C1(int _d) : d(_d) {
        std::cout << "c1: " << d << std::endl;
    }

    ~C1() {
        std::cout << "~C1: " << d << std::endl;
    }

    int d;
};

struct C2
{
    C2(int _d, int _d2) : c1(_d), c12(_d2) {
        std::cout << "C2" << std::endl;
    }

    ~C2() {
        std::cout << "~C2" << std::endl;
    }

    C1 c1;
    C1 c12;
};


int test_using()
{
    B<char>::Type b = 20;


    Derived d(10);
    std::cout << d.mD2  << std::endl;


    Aint<int> aint;
    std::cout << aint.ma << std::endl;

    C2 c2(1, 2);

    return 0;
}
