#include <iostream>
#include <memory>

using std::shared_ptr;

class A {
};

class B : public A
{

};

void func(const shared_ptr<B>& a)
{
    shared_ptr<A> sa = a;
}

int main(int argc, char *argv[])
{
    B *b = new B;
    func(shared_ptr<B>(b));

    std::cout << "hello ref" << std::endl;

    return 0;
}
