/*************************************************************************
 > File Name: stackQueue.cpp
 > Created Time: 2018年04月01日 星期日 21时49分10秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <stack>
#include <exception>

template <typename T>
class CQueue
{
public:
    CQueue();
    ~CQueue();

    void appendTail(const T& node);
    T deleteHead();

private:
    std::stack<T> stack1;
    std::stack<T> stack2;
};

template <typename T>
CQueue<T>::CQueue()
{

}

template <typename T>
CQueue<T>::~CQueue()
{

}

template <typename T>
void CQueue<T>::appendTail(const T& node)
{
    stack1.push(node);
}

template <typename T>
T CQueue<T>::deleteHead()
{
    T ret;

    if (stack2.size() > 0) {
        ret = stack2.top();
        stack2.pop();

        return ret;
    }

    if (stack1.size() == 0) {
        throw std::out_of_range(std::string("empty queue!"));
    }

    while (stack1.size() > 1) {
        stack2.push(stack1.top());
        stack1.pop();
    }

    ret = stack1.top();
    stack1.pop();

    return ret;
}



int main(int argc, char** argv)
{
    CQueue<int> q;
    q.appendTail(1);
    q.appendTail(2);
    q.appendTail(3);

    std::cout << q.deleteHead() << std::endl;
    q.appendTail(4);
    q.appendTail(5);
    std::cout << q.deleteHead() << std::endl;

    std::cout << "loop begin:" << std::endl;
    try {
        for (;;) {
            std::cout << q.deleteHead() << std:: endl;
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;        
    }

    return 0;
}
