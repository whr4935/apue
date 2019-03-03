/*************************************************************************
 > File Name: deleteNodeInList.cpp
 > Created Time: 2018年04月10日 星期二 10时48分10秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
   : mValue(v)
   , next(nullptr) {
    }

    int mValue;
    ListNode* next;
};

void deleteNode(ListNode** pListHead, ListNode* pToBeDeleted)
{
    if (pToBeDeleted->next == nullptr) {
        if (*pListHead == pToBeDeleted) {
            delete pToBeDeleted;
            *pListHead = nullptr;
            return;
        }

        ListNode* entry = nullptr;
        while ((entry = *pListHead)) {
            if (entry == pToBeDeleted) {
                *pListHead = entry->next;
                delete entry;
                return;
            }
            pListHead = &entry->next;
        }
    } else {
        ListNode* nextNode = pToBeDeleted->next;
        *pToBeDeleted = *nextNode;
        delete nextNode;
    }
}

void dump(ListNode* pListHead)
{
    std::cout << "dump list: ";
    while (pListHead) {
        std::cout << pListHead->mValue << " ";
        pListHead = pListHead->next;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    ListNode* const head = new ListNode(1); 
    ListNode* entry = new ListNode(2); 
    head->next = entry;
    ListNode* entry2 = new ListNode(3); 
    entry->next = entry2;
    ListNode* entry3 = new ListNode(4); 
    entry2->next = entry3;

    ListNode* h = head;
    deleteNode(&h, head);

    dump(h);


    return 0;
}

