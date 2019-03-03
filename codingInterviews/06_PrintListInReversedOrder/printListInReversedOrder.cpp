/*************************************************************************
 > File Name: 06_PrintListInReversedOrder/printListInReversedOrder.cpp
 > Created Time: 2018年03月28日 星期三 17时59分54秒
 ************************************************************************/

#include <iostream>
#include <stack> 
#include <Utilities/List.h>

void printListInReversedOrder(ListNode* pHead)
{
    std::stack<ListNode*> stack;

    while (pHead) {
        stack.push(pHead);
        pHead = pHead->m_pNext;
    }
    
    while (!stack.empty()) {
        auto p = stack.top();
        std::cout << p->m_nValue << std::endl;
        stack.pop();
    }
}

int main(int argc, char** argv)
{
    ListNode* node = createListNode(1);    
    addToTail(&node, 2);
    addToTail(&node, 3);
    addToTail(&node, 4);
    addToTail(&node, 5);

    // removeNode(&node, 4);
    // removeNode(&node, 1);

    printListInReversedOrder(node);

    // ListNode* n = node;
    // while (n) {
        // std::cout << n->m_nValue << std::endl;
        // n = n->m_pNext;
    // }


    return 0;
}

