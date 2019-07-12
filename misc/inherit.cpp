#include <iostream>

struct Base {
protected:
    int mData = 10;
};

struct Derive : Base {
    void print() {
        std::cout << "data = " << mData << std::endl;
        std::cout << "data = " << Base::mData << std::endl;
    }

protected:
    //int mData = 20;
};


int main(int argc, char *argv[])
{

    Derive d;

    d.print();
    
    return 0;
}
