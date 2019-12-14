/*************************************************************************
 > File Name: entryNodeInListLoop.cpp
 > Created Time: 2018年04月11日 星期三 21时09分00秒
 ************************************************************************/

#include <iostream>

struct ListNode {
    ListNode(int v)
        : mValue(v)
        , next(nullptr) {}
    int mValue;
    ListNode* next;
};

int checkLoopAndCount(ListNode* pHead)
{
    int count = 0;

    ListNode* fast = pHead, *slow = pHead;
    bool loop = false;

    while (fast) {
        fast = fast->next;
        if (fast) {
            if (fast == slow) {
                loop = true;
                break;
            }

            fast = fast->next;
            slow = slow->next;

            if (fast == slow) {
                loop = true;
                break;
            }
        } else {
            break;
        }
    }

    if (loop) {
        do {
            fast = fast->next;
            ++count;
        } while (fast != slow);
    }

    return count;
}

ListNode* entryNodeOfLoop(ListNode* pHead)
{
    if (pHead == nullptr)
        return nullptr;

    int count = checkLoopAndCount(pHead);
    if (count == 0)
        return nullptr;

    ListNode* fast = pHead, *slow = pHead;
    for (int i = 0; i < count; ++i) {
        fast = fast->next;
    }

    while (fast != slow) {
        fast = fast->next;
        slow = slow->next;
    }

    return slow;
}

void dump(ListNode* node, int n)
{
    std::cout << "dump: ";
    while (n--) {
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

    n[9]->next = n[1];

    dump(n[0], 10);

    //std::cout << checkLoopAndCount(n[0]) << std::endl;
    ListNode* entry = entryNodeOfLoop(n[0]);
    if (entry) {
        std::cout << entry->mValue << std::endl;
    } else {
        std::cout << "no loop" << std::endl;
    }

    return 0;
}

