#include <iostream>


class IBase
{





};


class A : public IBase
{

};


class B : public IBase
{

};


class Worker 
{
public:
    void setIBase(IBase* b)
    {
        mBase = b;
    }

    void fun()
    {
        std::cout << "hello: " << mBase << std::endl;
    }

private:
    IBase* mBase;
};


int main(int argc, char *argv[])
{
    Worker w;
    A a;
    w.setIBase(&a); 

    w.fun();


    return 0;
}
