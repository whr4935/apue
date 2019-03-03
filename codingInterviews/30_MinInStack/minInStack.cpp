/*************************************************************************
 > File Name: minInStack.cpp
 > Created Time: 2018年04月12日 星期四 22时15分34秒
 ************************************************************************/

#include <iostream>
#include <stack>

template <typename T>
class Stack {
public:
    Stack() {}
    ~Stack() {}

    void push(T d);
    T pop();
    T min();

private:
    std::stack<T> mData;
    std::stack<T> mMin;
};

template <typename T>
void Stack<T>::push(T d)
{
    mData.push(d);

    if (mMin.size()==0 || mMin.top() > d) {
        mMin.push(d);
    } else {
        mMin.push(mMin.top());
    }
}

template <typename T>
T Stack<T>::pop()
{
    mMin.pop();
    T d = mData.top();
    mData.pop();

    return d;
}

template <typename T>
T Stack<T>::min()
{
    T d = mMin.top();

    return d;
}


int main(int argc, char** argv)
{
    Stack<int> s;

    s.push(1);
    s.push(2);
    s.push(1);
    s.push(3);
    s.push(0);

    std::cout << s.min() << std::endl;
    s.pop();
    std::cout << s.min() << std::endl;
    s.pop();
    std::cout << s.min() << std::endl;

    s.pop();
    std::cout << s.min() << std::endl;

    s.pop();
    std::cout << s.min() << std::endl;


    return 0;
}
