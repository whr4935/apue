#ifndef _LIST_H
#define _LIST_H

#include <algorithm.h>

struct List {
    element_type pdata;
    int			key;
    struct List *next;
};

//////////////////////////////////////////////////////////////////////////
struct List *mallocListNode();
STATUS freeListNode(struct List *L);
STATUS initListNode(struct List *L, int key, element_type pdata);
STATUS initList(struct List **L);
struct List* listFindNode(struct List *L, int key);
struct List* listFindPreNode(struct List *L, struct List *node);
STATUS listAddNode(struct List **L, struct List *newNode);
STATUS listAddNodeTail(struct List **L, struct List *newNode);
STATUS ListDeleteNode(struct List **L, struct List *node);

#endif