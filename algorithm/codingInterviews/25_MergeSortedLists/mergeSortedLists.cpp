/*************************************************************************
 > File Name: mergeSortedLists.cpp
 > Created Time: 2018年04月11日 星期三 22时04分05秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
        : mValue(v)
        , next(nullptr) {}
    int mValue;
    ListNode* next;
};

void dump(ListNode* node)
{
    std::cout << "dump: ";
    while (node) {
        std::cout << node->mValue << " ";
        node = node->next;
    }
    std::cout << std::endl;
}

ListNode* mergeSortedLists(ListNode* l1, ListNode* l2)
{
    ListNode* head = nullptr;
    ListNode** pHead = &head;
    ListNode* node = nullptr;

    while (l1 && l2) {
        if (l1->mValue < l2->mValue) {
            node = l1;
            l1 = l1->next;
        } else {
            node = l2;
            l2 = l2->next;
        }
        *pHead = node;
        pHead = &(*pHead)->next;
    }

    while (l1) {
        *pHead = l1;
        pHead = &(*pHead)->next;
        l1 = l1->next;
    }

    while (l2) {
        *pHead = l2;
        pHead = &(*pHead)->next;
        l2 = l2->next;
    }

    return head;
}

int main(int argc, char** argv)
{
    ListNode* n1[5], *n2[5];
    for (int i = 0; i < 5; ++i) {
        ListNode* node = new ListNode(2*i);
        n1[i] = node;
        node = new ListNode(2*i+1);
        n2[i] = node;
    }

    for (int i = 3; i >= 0; --i) {
        n1[i]->next = n1[i+1];
        n2[i]->next = n2[i+1];
    }

    dump(n1[0]);
    dump(n2[0]);
    dump(mergeSortedLists(n1[0], n2[2]));

    return 0;
}

