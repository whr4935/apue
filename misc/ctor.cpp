#include <iostream>
#include <stdio.h>

struct A
{
//protected:
    A() {
    std::cout << "A" << std::endl;
    }
    ~A() {}
    //A(const A&) {}

    //A& operator=(const A&) {

    //}

    static void create();
};

struct B : A
{
//protected:
    B() {

    }

};

void A::create()
{
    A* a = new B();
}


A f1()
{
    return A();
}

int main(int argc, char *argv[])
{
    printf("%s\n", __TIMESTAMP__);
    printf("sequence.......\n");
    A a1;
    A a2 = a1;

    double d = 3.14;
    const int i  = d;

    //B b;
    const A& a3 = f1();
    
    return 0;
}
