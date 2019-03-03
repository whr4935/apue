/*************************************************************************
 > File Name: findKthToTail.cpp
 > Created Time: 2018年04月11日 星期三 17时43分32秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
    : mValue(v)
    , next(nullptr) {}
    int mValue;
    ListNode* next;
};

ListNode* findKthToTail(ListNode* pListHead, unsigned int k)
{
    if (pListHead == nullptr || k == 0)
        return nullptr;

    ListNode* pAhead = pListHead;
    ListNode* pBehind = nullptr;

    for (unsigned int i = 0; i < k-1; ++i) {
        if (!(pAhead = pAhead->next))
            return nullptr;
    }

    pBehind = pListHead;
    while (pAhead) {
        pAhead = pAhead->next;
        pBehind = pBehind->next;
    }

    return pBehind;
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
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    n1->next = n2;
    ListNode* n3 = new ListNode(3);
    n2->next = n3;
    ListNode* n4 = new ListNode(4);
    n3->next = n4;
    ListNode* n5 = new ListNode(5);
    n4->next = n5;
    ListNode* n6 = new ListNode(6);
    n5->next = n6;

    dump(n1);
    ListNode* p = findKthToTail(n1, 6);
    if (p) {
        std::cout << p->mValue << std::endl;
    } else {
        std::cout << "find failed!" << std::endl;
    }

    return 0;
}

