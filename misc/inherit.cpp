#include <iostream>

struct Base {
    void base_func() {
        std::cout << __FUNCTION__ << std::endl;
    }
    virtual void foo(int a = 3) {
        std::cout << "Base foo" << std::endl;
    }
protected:
    int mData = 10;

//private:
    //Base();
};

struct Derive : Base {
    void print() {
        std::cout << "data = " << mData << std::endl;
        std::cout << "data = " << Base::mData << std::endl;
    }

    void dereive_func() {
        std::cout << __FUNCTION__ << std::endl;
    }

    virtual void foo(int a) {
        std::cout << "Derive foo" << std::endl;
    }

    virtual void foo(double a) {
        std::cout << "Derive double foo" << std::endl;
    }

    Derive() {

    }

protected:
    //int mData = 20;
};


int main(int argc, char *argv[])
{

    Derive d;
    d.foo(1.0);
    d.print();
    d.base_func();

    Base& b = d;
    b.foo();
    
    return 0;
}
