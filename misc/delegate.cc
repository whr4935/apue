#include <iostream>

struct C1
{
    int id = 1;

    void f1() {
        std::cout << "id=" << id << " " << std::endl;
    }
};


struct C2
{
    int id = 2;
    
    void f2(int d) {
        std::cout << "id=" << id << " , f2 d=" << d << std::endl;
    }
};

struct dummy {
};


int main(int argc, char *argv[])
{
    typedef void (dummy::*PF)(int);
    C2 c2;
    dummy* c1 = reinterpret_cast<dummy*>(&c2);
    int64_t a;
    printf("%ld", a);
    
    PF f = (PF)&C2::f2;

    (c1->*f)(123);
    
    return 0;
}
