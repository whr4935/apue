/*************************************************************************
 > File Name: reverseList.cpp
 > Created Time: 2018年04月11日 星期三 21时47分46秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
        : mValue(v)
        , next(nullptr) {}
    int mValue;
    ListNode* next;
};

ListNode* reverseList(ListNode* pHead)
{
    ListNode* pre = nullptr;
    ListNode* cur = pHead;
    ListNode* nxt  = nullptr;

    while (cur) {
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }

    return pre;
}

void dump(ListNode* node)
{
    std::cout << "dump: ";
    while (node) {
        std::cout << node->mValue << " ";
        node = node->next;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    ListNode* n[10];
    for (int i = 0; i < 10; ++i) {
        ListNode* node = new ListNode(i+1);
        n[i] = node;
    }

    for (int i = 8; i >= 0; --i) {
        n[i]->next = n[i+1];
    }

    dump(n[0]);
    dump(reverseList(n[0]));

    return 0;
}

