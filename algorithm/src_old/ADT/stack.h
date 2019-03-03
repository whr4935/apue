#ifndef _STACK_H
#define _STACK_H

#include <algorithm.h>

struct Stack {
    element_type	data;
    struct Stack	*next;
};

//////////////////////////////////////////////////////////////////////////
struct Stack* mallocStackNode();
STATUS freeStackNode(struct Stack *node);
STATUS initStack(struct Stack **S);
STATUS push(struct Stack **S, element_type data);
STATUS pop(struct Stack **S, element_type *pdata);
STATUS top(struct Stack *S, element_type *pdata);
BOOL stackIsEmpty(struct Stack *S);
BOOL stackIsFull(struct Stack *S);

#endif