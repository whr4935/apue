#include <iostream>


class A
{
public:
    //virtual ~A() {}
    void func() {
        std::cout << &A::a1 <<std::endl;
        std::cout << &(A::a1) <<std::endl;
        std::cout << sizeof(&A::func) << std::endl;
        std::cout << "this: " <<this <<std::endl;
    }

    int a1 = 100;
};








int main(int argc, char *argv[])
{
    A a;
    //a.func();
    //(void (A::*p)()) = &A::func;
    typedef void (A::*PFunc)();
    PFunc p = &A::func;
    //(a.*p)();
    void* p2 = (void*&)p;
    ((void (*)())p2)();
    //std::cout << p2 << " "<< p <<std::endl;

    int A::* pi = &A::a1;
    int* pi2 = &a.a1;
    std::cout << a.*pi <<std::endl; 
    //std::cout << pi2 <<std::endl; 
    //std::cout << &a <<std::endl; 

    int aa = 3;
    auto b = (int*&&)&aa;
    std::cout <<*b <<std::endl;

    //__type_traits<int>::is_POD_type;
    //std::is_pod<int>;
    std::cout << __is_pod(int) <<std::endl;
    std::cout << __is_pod(A) <<std::endl;
    return 0;
}
