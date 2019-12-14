/*************************************************************************
 > File Name: queueStack.cpp
 > Created Time: 2018年04月01日 星期日 22时44分08秒
 ************************************************************************/

#include <iostream>
#include <exception>
#include <queue>

template <typename T>
class CStack
{
public:
    CStack();
    ~CStack();

    void push(const T& node);
    T top();
    void pop();

private:
    std::queue<T> queue1;
    std::queue<T> queue2;
    std::queue<T> *f, *b;
};

template <typename T>
CStack<T>::CStack()
    : f(nullptr)
    , b(nullptr)
{

}

template <typename T>
CStack<T>::~CStack()
{

}

template <typename T>
void CStack<T>::push(const T& node)
{
    std::queue<T> *q = nullptr;
    if (f) {
        q = f;
    } else {
        q = queue2.size() > 0 ? &queue2 : &queue1;
    }
    
    q->push(node);
}

template <typename T>
T CStack<T>::top()
{
    if (f) {
        if (f->size() != 1) {
            throw std::logic_error("front queue size != 1");
        }

        return f->front();
    }

    if (queue2.size() > 0) {
        f = &queue2;
        b = &queue1;
    } else {
        f = &queue1;
        b = &queue2;
    }

    if (f->empty()) {
        throw std::out_of_range("empty stack");
    }

    T tmp;
    while (f->size() > 1) {
        tmp = f->front();
        f->pop();
        b->push(tmp);
    }
    
    T ret = f->front();
    return ret;
}

template <typename T>
void CStack<T>::pop()
{
    if (f->size() == 0) {
        //throw std::logic_error("pop error!");
        return;
    }

    if (!f || f->size() > 1) {
        top();
    }

    f->pop();
    f = nullptr;
}

int main(int argc, char** argv)
{
    CStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.top();
    std::cout << s.top() << std::endl, s.pop();
    std::cout << s.top() << std::endl, s.pop();
    s.push(4);
    std::cout << s.top() << std::endl, s.pop();
    std::cout << s.top() << std::endl, s.pop();
    try {
        std::cout << s.top() << std::endl, s.pop();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }


    return 0;
}
