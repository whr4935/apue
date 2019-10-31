#include "DeferredActions.h"
#include <iostream>

struct A
{
    void f1(int& a) {
        a = 10888;
        std::cout << __PRETTY_FUNCTION__ << " " << a << std::endl;
    }

    virtual void print() {
        std::cout << "print:aaa" << std::endl;
    }
};

struct B : A
{
    void print() {
        std::cout <<"print:bbb" << std::endl;
    }
};

struct C : B
{
    void print() {
        std::cout <<"print:ccc" << std::endl;
    }
};

void testFunc(std::shared_ptr<A>& data)
{
    data->print();
    std::cout << __LINE__ << ": data.use_count = " << data.use_count() << std::endl;
}

template <typename...Args>
void Call(void(*p)(Args...), Args&&... args)
{
    auto f = std::bind(p, std::forward<Args&&>(args)...); 
    f();
}

template <typename T>
void Call2(std::shared_ptr<T> d)
{
    std::cout << "ref count: " << d.use_count() << std::endl;
}

void testDeferredActions()
{
    ActionProcesser a;

    a.add([]{std::cout << "hello" << std::endl;});


    std::shared_ptr<A> a1(new A);
    int arg1 = 12;
    a.add(makeAction(&A::f1, std::weak_ptr<A>(a1), arg1));



    a.run();

    std::cout << "arg1: " << arg1 << std::endl;
    std::cout << "testDeferredActions finished!" << std::endl;

    Call(testFunc, a1);

    Call2(a1);

    C b;
    auto testVirtuaalFuncBind = std::bind(&A::print, b);
    testVirtuaalFuncBind();

}
