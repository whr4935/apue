#include <iostream>

struct Base
{
protected:
//private:
    int mData = 10;
};

//struct A;
struct Derived : public Base
{
    friend struct A;
};

struct A
{
    void test(Derived* d)
    {
        d->mData = 12;
    }
};


int main(int argc, char *argv[])
{
    Derived d;
    A a;
    a.test(&d);

    return 0;
}
