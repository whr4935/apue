#include "threadpool.h"
#include <gtest/gtest.h>

class TestThreadPool : public testing::Test {
public:
    TestThreadPool() 
    //: mThreadPool(2) 
    {

    }

    void test() {

    }
    
protected:
    ThreadPool mThreadPool{2};
};

TEST_F(TestThreadPool, threadPool)
{
    this->mThreadPool.commit([](int a, int b) {
            std::cout << std::this_thread::get_id() << std::endl;
        std::cout << "a = " << a << " , b = " << b << std::endl;
    },
                             1, 2);
}

TEST_F(TestThreadPool, threadpool2)
{

}

class A : public testing::Test
{
public:
    A()
    : a(getA2())
    {
    }

private:
    static const int getA()
    {
        std::cout << "getA" << std::endl;
        return 10;
    }

    static const int getA2()
    {
        std::cout << "getA2" << std::endl;
        return 10;
    }

    int a = getA();
};

TEST_F(A, one)
{

}
