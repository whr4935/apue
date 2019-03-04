#include <iostream>

class Base
{
    virtual void func(int) = 0;

protected:
    int base1 = 81;
};

class A : virtual public Base
{
public:
    virtual void fff() = 0;
    virtual void func(int) {

    }

private:
    int a1 = 100;
    int a2 = 101;
};


class B : virtual public Base
{
public:
    virtual void fff(int) {
        std::cout << "B::fff int" << std::endl;
    }

    virtual void func(int) {

    }
    int b1 = 200;
};

class C : public A, public B
{
public:

    virtual void func(int d) {
        std::cout << "func " << d << std::endl;
    }

    void fff() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        base1;
    }

    //virtual void func(double d) {
        //std::cout << "func " << d << std::endl;
    //}

    int c1 = 300;
};

int main(int argc, char *argv[])
{
    C c;
    A& a = c;
    a.func(1);
    a.fff();
    
    B& b = c;
    b.func(2);
    b.fff(2);

    long* p = (long*)&c;
    std::cout << std::hex << *p << std::dec << std::endl;
    std::cout << sizeof(A) << std::endl;

    int* ip = (int*)(p+1);
    //++ip;
    std::cout << *ip++ << std::endl;
    std::cout << *ip++ << std::endl;
    //++ip;
    ++ip;
    ++ip;
    std::cout << *ip++ << std::endl;
    //++ip;
    //++ip;
    //++ip;
    std::cout << *ip++ << std::endl;
    std::cout << "sizeof B" << sizeof(B) << std::endl;
    std::cout << "sizeof C" << sizeof(C) << std::endl;
    ++ip;
    ++ip;
    std::cout << *ip++ << std::endl;

    void** vp = *(void***)p;
    //((void (*)(int))vp[0])(1);

    //void (* pf)(int) = (void (*)(int))vp;
    //pf(1);

    int C::* cp = &C::c1;
    c.*cp = 120;
    //std::cout << c.c1 << std::endl;

    void (C::*pp1)(int) = &C::func;
    //std::cout << pp1 << std::endl;

    return 0;
}
