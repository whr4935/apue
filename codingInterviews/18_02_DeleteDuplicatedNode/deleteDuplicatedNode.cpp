/*************************************************************************
 > File Name: deleteDuplicatedNode.cpp
 > Created Time: 2018年04月11日 星期三 08时00分14秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
    : mValue(v)
    , next(nullptr) {}

    int mValue;
    ListNode* next;
};

void deleteDuplicatedNode(ListNode** pListHead)
{
    ListNode** &node = pListHead;

    while (*node) {
        int v = (*node)->mValue;

        ListNode* entry = (*node)->next;
        if (entry && entry->mValue == v) {
            do {
                delete *node;
                *node = entry;
                entry = (*node)->next;
            } while (entry && entry->mValue == v);

            delete *node;
            *node = entry;
        } else {
            node = &(*node)->next;
        }
    }
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
    ListNode* n2 = new ListNode(1); 
    n1->next = n2;
    ListNode* n3 = new ListNode(3); 
    n2->next = n3;
    ListNode* n4 = new ListNode(4); 
    n3->next = n4;

    deleteDuplicatedNode(&n1);

    dump(n1);

    return 0;
}
