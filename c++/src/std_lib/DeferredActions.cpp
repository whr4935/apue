#include "DeferredActions.h"
#include <iostream>

struct A
{
    void f1(int& a) {
        a = 10888;
        std::cout << __PRETTY_FUNCTION__ << " " << a << std::endl;
    }
};


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
}
