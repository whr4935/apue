#include "list.h"

struct List *mallocListNode()
{
    struct List *node = NULL;

    node = (struct List *)malloc(sizeof(struct List));

    return node;
}

STATUS freeListNode(struct List *L)
{
    if (L == NULL)
        return FAILURE;

    free(L);
    return SUCCESS;
}

STATUS initListNode(struct List *L, int key, element_type pdata)
{
    if (L == NULL)
        return FAILURE;

    L->next = NULL;
    L->key = key;
    L->pdata = pdata;

    return SUCCESS;
}

STATUS initList(struct List **L)
{
    *L = NULL;

    return SUCCESS;
}

struct List* listFindNode(struct List *L, int key)
{
    while (L) {
        if (L->key == key) {
            break;
        }
        L = L->next;
    }

    return L;
}

struct List* listFindPreNode(struct List *L, struct List *node)
{
    if (L == node)
        return NULL;

    while (L) {
        if (L->next == node) {
            break;
        }
        L = L->next;
    }

    return L;
}

STATUS listAddNode(struct List **L, struct List *newNode)
{
    newNode->next = *L;
    *L = newNode;

    return SUCCESS;
}

STATUS listAddNodeTail(struct List **L, struct List *newNode)
{
    while (*L) {
        L = &(*L)->next;
    }

    newNode->next = *L;
    *L = newNode;

    return SUCCESS;
}

STATUS ListDeleteNode(struct List **L, struct List *node)
{
    struct List **ppNode = L;

    while (*ppNode) {
        if (*ppNode == node) {
            *ppNode = (*ppNode)->next;

            return freeListNode(node);
        }

        ppNode = &(*ppNode)->next;
    }

    return FAILURE;
}

STATUS printList(struct List *L, const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    while (L) {
        printf("%d ", L->key);
        L = L->next;
    }
    printf("\n");

    return SUCCESS;
}


int TEST_list()
{
    struct List *head = NULL;
    struct List *node = NULL;
    struct List *preNode = NULL;
    int i;
    element_type data = 0;
    static int gen_key;
    int key;

    for (i = 0; i < 10; ++i) {
        node = mallocListNode();
        CU_ASSERT_PTR_NOT_NULL(node);
        data = (element_type)(getRandom()&0x7F);
        initListNode(node, ++gen_key, data);

        if (listAddNodeTail(&head, node) != SUCCESS) {
            freeListNode(node);
        }
    }
    printList(head, "List print: ");

    key = 4;
    node = listFindNode(head, key);
    if (node) {
        printf("find node: key = %d\n", (int)node->key);
    } else {
        printf("can't find the node!\n");
    }

    preNode = listFindPreNode(head, node);
    if (preNode) {
        printf("find pre node: key = %d, pre node key = %d\n", node->key, preNode->key);
    } else {
        printf("can't find the pre node!\n");
    }

    ListDeleteNode(&head, node);
    ListDeleteNode(&head, preNode);

    printList(head, "After delete: ");

    return 0;
}

//////////////////////////////////////////////////
CU_TestInfo TEST_SUITE_list[] = {
    TEST_CASE_ENTRY(TEST_list),
    CU_TEST_INFO_NULL
};
