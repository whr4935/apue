/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

struct ComplexListNode {
    ComplexListNode(int v)
    : mValue(v)
    , next(nullptr)
    , sibling(nullptr) {}

    ComplexListNode(const ComplexListNode& o)
    : mValue(o.mValue)
    , next(o.next)
    , sibling(o.sibling)
    {
    }

    int mValue;
    ComplexListNode* next;
    ComplexListNode* sibling;
};

ComplexListNode* clone(ComplexListNode* pHead)
{
    ComplexListNode* pCloneHead = nullptr;
    ComplexListNode** pp = &pCloneHead;
    ComplexListNode *n = pHead;
    ComplexListNode* entry = nullptr, *newNode = nullptr;

    while (n) {
        entry = n;
        n = entry->next;

        newNode = new ComplexListNode(*entry);
        entry->next = newNode; 
    }

    n = pHead;
    while (n) {
        entry = n;
        n = n->next->next;

        if (entry->sibling) {
            entry->next->sibling = entry->sibling->next;
        }
    }

    n = pHead;
    while (n) {
        entry = n;
        n = n->next->next;
        *pp = entry->next;
        pp = &(*pp)->next;
        entry->next = n;
    }

    return pCloneHead;
}

void dump(ComplexListNode* pHead)
{
    std::cout << "dump: \n";
    while (pHead) {
        std::cout << pHead->mValue << " ";
        if (pHead->sibling) {
            std::cout << "sibling: " << pHead->sibling << " (" << pHead->sibling->mValue << ")";
        }
        std::cout << "\n";
        pHead = pHead->next;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    ComplexListNode* head = new ComplexListNode(1);
    ComplexListNode* n2 = new ComplexListNode(2);
    ComplexListNode* n3 = new ComplexListNode(3);
    ComplexListNode* n4 = new ComplexListNode(4);
    ComplexListNode* n5 = new ComplexListNode(5);
    ComplexListNode* n6 = new ComplexListNode(6);

    head->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;

    head->sibling = n3;
    n2->sibling = n5;
    n4->sibling = n2;

    auto p = clone(head);
    dump(head);

    std::cout << "==============\n";
    dump(p);

    return 0;
}
