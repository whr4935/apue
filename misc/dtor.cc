#include <iostream>

struct D
{
    D() {
        std::cout << "D" << std::endl;
    }

    ~D() {
        std::cout << "~D" <<std::endl;
    }
};


struct B {
    B() {
        std::cout << "B" <<std::endl;
    }

    ~B() {
    std::cout << "~B" << std::endl;
    }

    D d;
};

class C {

    ~C() {

    }
public:
    static C* getInstance() {
        return new C();
    }

    int a = 10;
};

struct A1 {
    virtual ~A1() {
        std::cout << "A1" << std::endl;
    }
};

struct A2 : A1 {
    ~A2() {
        std::cout << "A2" <<std::endl;
    }
};

struct A3 : A2 {
    ~A3() {
        std::cout << "A3" << std::endl;
    }
};


int main(int argc, char *argv[])
{
    //B(); 

    //C *c  = C::getInstance();
    //c->a = 20;
    //delete c;


    A2* a = new A3();
    delete a;

    return 0;
}
