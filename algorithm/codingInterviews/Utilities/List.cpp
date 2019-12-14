/*************************************************************************
 > File Name: Utilities/List.cpp
 > Created Time: 2018年03月28日 星期三 17时13分05秒
 ************************************************************************/

#include "List.h"

ListNode* createListNode(int value)
{
    ListNode* node = new ListNode;
    if (node != nullptr) {
        node->m_nValue = value;
        node->m_pNext = nullptr;
    }
    return node;
}

void addToTail(ListNode** pHead, int value)
{
    while (*pHead) {
        pHead = &(*pHead)->m_pNext;
    }

    ListNode* node = createListNode(value);
    *pHead = node;
}

void removeNode(ListNode** pHead, int value)
{
    ListNode* entry = nullptr;

    while ((entry = *pHead)) {
        if (entry->m_nValue == value) {
            *pHead = entry->m_pNext;
            delete entry;
            break;
        }
        pHead = &entry->m_pNext;
    }
}


