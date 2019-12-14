/*************************************************************************
 > File Name: Utilities/List.h
 > Created Time: 2018年03月28日 星期三 17时08分48秒
 ************************************************************************/

#ifndef _UTILITIES_LIST_H
#define _UTILITIES_LIST_H

struct ListNode {
    int m_nValue;
    ListNode* m_pNext;
};

ListNode* createListNode(int value);
void connectListNode(ListNode* pCurrent, ListNode* pNext);
void printListNode(ListNode* pNode);
void printList(ListNode* pHead);
void destroyList(ListNode* pHead);
void addToTail(ListNode** pHead, int value);
void removeNode(ListNode** pHead, int value);


#endif
