#include <iostream>

struct Base
{
    int mData = 10; 
};

struct A1 : virtual public Base
{
    virtual void t1() {
        std::cout << "A1 t1 void" <<std::endl;
    }

    virtual void t1(int ) {
        //std::cout << "A1 t1 int" <<std::endl;
        t1();
    }
};


struct B1: virtual public Base
{

};

struct A2 :public A1
{
    //virtual int t1() {
        //std::cout << "A1 t1" <<std::endl;
    //}

    virtual void t1(int ) {
        std::cout << "A2 t1 int" <<std::endl;
        A1::t1(1);
    }

};

struct A3 :public A2
{
    virtual void t1() {
        std::cout << "A3 t1 void" <<std::endl;
        //A1* p = this;
        //p->t1(2);

    }

    //virtual void t1(int ) {
        //std::cout << "A2 t1 int" <<std::endl;
    //}

};

struct B2: public B1
{

};

struct C :public A2, public B2
{

};


int main(int argc, char *argv[])
{
    A3 a;
    A1* a1 = &a;
    a1->t1(1);


    C c; 
    std::cout <<c.mData <<std::endl;
    return 0;
}
