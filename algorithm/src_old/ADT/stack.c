#include "stack.h"

struct Stack* mallocStackNode()
{
    struct Stack *node;

    node = (struct Stack *)calloc(1, sizeof(struct Stack));

    return node;
}


STATUS freeStackNode(struct Stack *node)
{
    if (node == NULL)
        return FAILURE;

    free(node);
    return SUCCESS;
}

STATUS initStackNode(struct Stack *node, element_type data)
{
    node->data = data;
    node->next = NULL;

    return SUCCESS;
}

STATUS initStack(struct Stack **S)
{
    *S = NULL;

    return SUCCESS;
}

STATUS push(struct Stack **S, element_type data)
{
    struct Stack *node;

    if (stackIsFull(*S))
        return FAILURE;

    node = mallocStackNode();
    if (node == NULL)
        return FAILURE;

    initStackNode(node, data);

    node->next = *S;
    *S = node;

    return SUCCESS;
}

STATUS pop(struct Stack **S, element_type *pdata)
{
    struct Stack *node;

    if (stackIsEmpty(*S)) {
        *pdata = 0;
        return FAILURE;
    }

    node = *S;
    *S = node->next;
    *pdata = node->data;

    freeStackNode(node);

    return SUCCESS;
}

STATUS top(struct Stack *S, element_type *pdata)
{
    if (stackIsEmpty(S)) {
        *pdata = 0;
        return FAILURE;
    }

    *pdata = S->data;

    return SUCCESS;
}

BOOL stackIsEmpty(struct Stack *S)
{
    return S == NULL;
}

BOOL stackIsFull(struct Stack *S)
{
    return FALSE;
}

STATUS printStack(struct Stack *S, char *fmt, ...)
{
    va_list ap;
    struct Stack *node;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    node = S;
    while (node) {
        printf("%d ", (int)node->data);
        node = node->next;
    }
    printf("\n");

    return SUCCESS;
}

int TEST_stack()
{
    struct Stack *S = NULL;
    element_type test_data[10];
    int i;
    int count = 0;
    int count2 = 0;
    element_type data;
    STATUS ret;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = i + 1;
    }

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        push(&S, test_data[i]);
    }

    printStack(S, "PrintStack <%d>: ", ++count);

    ret = top(S, &data);
    if (ret == SUCCESS) {
        printf("top data = %d\n", data);
    }

    for (i = 0; i < 5; ++i) {
        pop(&S, &data);
    }
    printStack(S, "PrintStack <%d>: ", ++count);


     return 0;
}

CU_TestInfo TEST_SUITE_stack[] = {
    TEST_CASE_ENTRY(TEST_stack),
    CU_TEST_INFO_NULL
};

//////////////////////////////////////////////////////////////////////////
#if 0
struct STACK {
    Dataptr		pdata;
    struct STACK *next;
};

struct STACK*  mallocStackNode()
{
    struct STACK *node;

    node = calloc(1, sizeof(struct STACK));

    return node;
}

STATUS freeStackNode(struct STACK *node)
{
    if (node) {
        free(node);
        return SUCCESS;
    }

    return FAILURE;
}

STATUS initStackNode(struct STACK *node, Dataptr pdata)
{
    node->pdata = pdata;
    node->next = NULL;

    return SUCCESS;
}

STATUS pushStack(struct STACK **S, Dataptr pdata)
{
    struct STACK *node;

    node = mallocStackNode();

    if (node) {
        initStackNode(node, pdata);

        node->next = (*S);
        (*S) = node;

        return SUCCESS;
    }

    return FAILURE;
}

STATUS popStack(struct STACK **S, Dataptr *p)
{
    struct STACK *node;

    if ((*S)) {
        node = (*S);
        (*S) = node->next;
        *p = node->pdata;

        freeStackNode(node);

        return SUCCESS;
    }

    *p = INVALID_DATA;
    return FAILURE;
}

Dataptr topStack(struct STACK *S)
{
    if (S) {
        return S->pdata;
    }

    return INVALID_DATA;
}

STATUS printStack(struct STACK *S, char *fmt, ...)
{
//	int i;
    va_list ap;
    struct STACK *node;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    node = S;
    while (node) {
        printf("%d ", (int)node->pdata);
        node = node->next;
    }
    printf("\n");

    return SUCCESS;
}

int TEST_stack()
{
    struct STACK *S = NULL;
    int test_data[10];
    int i;
    int count = 0;
    int count2 = 0;
    Dataptr pdata;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = i + 1;
    }

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        pushStack(&S, (Dataptr)test_data[i]);
    }
    printStack(S, "PrintStack S1 <%d>: ", ++count);


    struct STACK *S2 = NULL;
    while (popStack(&S, &pdata) == SUCCESS) {
        pushStack(&S2, pdata);
    }
    printStack(S2, "PrintStack S2 <%d>: ", ++count2);

    return 0;
}
#endif
