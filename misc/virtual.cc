#include <iostream>
#include <typeinfo>

#define NOT_IMPLEMENTED() printf("%s Not Implemented!\n", __FUNCTION__) 

struct Base
{
    Base(int d)
    : mData(d)
    {
        std::cout << "Base(), mData = " << mData << std::endl;
    }

    int mData = 10; 
};

struct A1 : virtual public Base
{
    A1()
    : Base(1)
    {
        std::cout << "A1()" << std::endl;
    }

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
    B1()
    : Base(11)
    {

    }

};

struct A2 :public A1
{
    //virtual int t1() {
        //std::cout << "A1 t1" <<std::endl;
    //}

    A2(int d) 
    : Base(d)
    {
        std::cout << "A2()" << std::endl;
    }

    virtual void t1(int ) {
        std::cout << "A2 t1 int" <<std::endl;
        A1::t1(1);
    }

};

struct A3 :public A2
{
    A3()
    : A2(3)
    , Base(3)
    {

    }
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
    B2()
    : Base(12)
    {

    }

};

struct C :public A2, public B2
{
    C() : Base(3), A2(3), mArray{1, 2, 3} {}
    void test() {
        printf("%d %d %d\n", mArray[0], mArray[1], mArray[2]);
    }

    int mArray[3];
};

void virtual_func(int a = 10)
{

}


int main(int argc, char *argv[])
{
    {
        A2 a2(109);
        exit(0);
    }


    A3 a;
    A1* a1 = &a;
    a1->t1(1);

    NOT_IMPLEMENTED();




    C c; 
    std::cout <<c.mData <<std::endl;
    c.test();

    bool ret = typeid(A3) == typeid(*a1);
    std::cout << "ret = " <<ret << std::endl;
    std::cout << typeid(*a1).name() << std::endl;

    auto p = dynamic_cast<A2*>(a1);
    std::cout << "p = " << p <<std::endl;

    return 0;
}
