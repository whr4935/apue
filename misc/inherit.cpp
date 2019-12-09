#include <iostream>

struct Base {
    void base_func() {
        std::cout << __FUNCTION__ << std::endl;
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

    Derive() {

    }

protected:
    //int mData = 20;
};


int main(int argc, char *argv[])
{

    Derive d;

    d.print();
    d.base_func();
    
    return 0;
}
