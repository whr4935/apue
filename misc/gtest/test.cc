#include <gtest/gtest.h>
#include <iostream>
#include <list>

int factorial(int n)
{
    if (n == 0)
        return 1;
    return factorial(n-1)*n;
}

TEST(factorialTest, testZero)
{
    EXPECT_EQ(factorial(0), 1);
}

TEST(factorialTest, testPositive)
{
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(3), 3);
}


template <typename E>
class Queue
{
public:
    Queue();
    ~Queue();
    void enQueue(const E& element);
    E deQueue();
    size_t size() const;

private:
    std::list<E> mElements;
    size_t msize;
};

template<typename E>
Queue<E>::Queue()
    : msize(0)
{

}

template<typename E>
Queue<E>::~Queue()
{
}

template<typename E>
void Queue<E>::enQueue(const E& element)
{
    mElements.push_back(element);
}

template<typename E>
E Queue<E>::deQueue()
{
    E e = mElements.front();
    mElements.pop_front();

    return e;
}

template<typename E>
size_t Queue<E>::size() const
{
    return mElements.size();
}

class queueTest : public testing::Test
{
protected:
    void SetUp() override
    {
        q1.enQueue(1);
        q1.enQueue(2);
    }


    void TearDown() override
    {
        int d = q1.deQueue();        
    }

    Queue<int> q1;
    Queue<int> q2;
    Queue<int> q3;
};

TEST_F(queueTest, enQueuTest)
{
    EXPECT_EQ(q1.size(), 1);
}

TEST_F(queueTest, deQueueTest)
{
    q1.deQueue();
    EXPECT_EQ(q1.size(), 1);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
