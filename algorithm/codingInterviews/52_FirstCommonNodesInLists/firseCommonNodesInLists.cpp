/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

struct ListNode {
    ListNode(int v)
    : mValue(v)
    , next(nullptr) {}

    int mValue;
    ListNode* next;
};

ListNode* findFirstCommonNode(ListNode* pHead1, ListNode* pHead2)
{
    ListNode* p1 = pHead1, *p2 = pHead2;
    ListNode* longer = nullptr;
    ListNode* shorter = nullptr;
    int diff = 0;

    while (p1 || p2) {
        if (p1) p1 = p1->next;
        if (p2) p2 = p2->next;
        if ((p1 == nullptr) ^ (p2 == nullptr)) {
            ++diff;
            longer = p1 == nullptr ? pHead2 : pHead1;
            shorter = p1 == nullptr ? pHead1 : pHead2;
        }
    }
    std::cout << "diff length: " << diff << std::endl;

    while (diff--) {
        longer = longer->next;
    }

    while (longer != shorter) {
        longer = longer->next;
        shorter = shorter->next;
    }

    return longer;
}

int main(int argc, char *argv[])
{
    ListNode* n1 = new ListNode(1); 
    ListNode* n2 = new ListNode(2); 
    ListNode* n3 = new ListNode(3); 
    ListNode* n4 = new ListNode(4); 
    ListNode* n5 = new ListNode(5); 
    ListNode* n6 = new ListNode(6); 
    ListNode* n7 = new ListNode(7); 

    n1->next = n2;
    n2->next = n3;
    n3->next = n6;
    n6->next = n7;

    n4->next = n5;
    n5->next = n6;

    ListNode* n = findFirstCommonNode(n1, n4);
    if (n) {
        std::cout << n->mValue << std::endl;
    } else {
        std::cout << "not found!" << std::endl;
    }
    
    return 0;
}
