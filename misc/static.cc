#include <iostream>

class Obj
{
public:
    Obj() {
        std::cout << "Obj ctor" << std::endl;
    }
};


class A {
public:
    static Obj o;

    void f() {
        std::cout << "A::f" <<std::endl;
        static Obj o;
    }
};

Obj A::o;


int main(int argc, char *argv[])
{
    std::cout << "in main" << std::endl;
    A a;
    a.f();

    return 0;
}
