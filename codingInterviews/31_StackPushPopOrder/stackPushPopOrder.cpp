/*************************************************************************
 > File Name: stackPushPopOrder.cpp
 > Created Time: 2018年04月12日 星期四 22时32分30秒
 ************************************************************************/

#include <iostream>
#include <stack>

bool isPopOrder(const int* pPush, const int* pPop, int length)
{
    const int *pPushEnd = pPush + length;
    const int *pPopEnd = pPop + length;
    std::stack<int> s;

    while (pPop < pPopEnd) {
        if (s.size()==0 || *pPop != s.top()) {
            for (;;) {
                if (pPush == pPushEnd)
                    return false;

                if (*pPush != *pPop) {
                    s.push(*pPush++);
                } else {
                    ++pPush;
                    break;
                }
            }
        } else {
            s.pop();
        }

        ++pPop;
    }

    return true;
}

int main(int argc, char** argv)
{
    int push[] = {1, 2, 3, 4, 5};
    int pop1[] = {4, 5, 3, 2, 1};
    int pop2[] = {4, 3, 5, 1, 2};

    std::cout << std::boolalpha << isPopOrder(push, pop1, 5) << std::endl;
    std::cout << std::boolalpha << isPopOrder(push, pop2, 5) << std::endl;

    return 0;
}

