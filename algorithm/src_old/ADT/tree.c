#include "tree.h"
#include "stack.h"
#include "queue.h"
#include <misc/misc.h>

struct Tree* mallocTreeNode()
{
    struct Tree* node;

    node = (struct Tree*)calloc(1, sizeof(struct Tree));

    return node;
}

STATUS freeTreeNode(struct Tree* node)
{
    if (node == NULL)
        return FAILURE;

    free(node);

    return SUCCESS;
}

STATUS initTreeNode(struct Tree* node, element_type data)
{
    node->data = data;
    node->next_sibling = NULL;
    node->first_child = NULL;

    return SUCCESS;
}

STATUS initTree(struct Tree **T)
{
    *T = NULL;

    return SUCCESS;
}

STATUS preTraverseTree(struct Tree *T)
{
    struct Tree *node;
    element_type data;
    struct Stack *S;

    printf("pre  traverse: ");
    initStack(&S);

#if 0
    push(&S, T);

    while (!stackIsEmpty(S)) {
        pop(&S, &data);
        printf("%c ", ((struct Tree*)data)->data);

        if (((struct Tree*)data)->next_sibling)
            push(&S, (element_type)((struct Tree*)data)->next_sibling);
        if (((struct Tree*)data)->first_child)
            push(&S, (element_type)((struct Tree*)data)->first_child);
    }
#else
    node = T;
    while (node || !stackIsEmpty(S)) {
        while (node) {
            printf("%c ", node->data);

            if (node->next_sibling) {
                push(&S, (element_type)node->next_sibling);
            }

            node = node->first_child;
        }

        pop(&S, &data);
        node = (struct Tree *)data;
    }

#endif

    printf("\n");
    return SUCCESS;
}

STATUS inTraverseTree(struct Tree *T)
{
    element_type data;
    struct Tree *node;
    struct Stack *S;

    initStack(&S);
    node = T;

    printf("in   traverse: ");

    while (node || !stackIsEmpty(S)) {
        while (node) {
            push(&S, (element_type)node);
            node = node->first_child;
        }
        pop(&S, &data);
        node = (struct Tree*)data;
        printf("%c ", node->data);
        node = node->next_sibling;
    }

    printf("\n");
    return SUCCESS;
}

STATUS postTraverseTree(struct Tree *T)
{
    element_type data;
    struct Tree *node;
    struct Stack *S;

    initStack(&S);
    node = T;

    printf("post traverse: ");
    while (node || !stackIsEmpty(S)) {
        while (node) {
            push(&S, (element_type)node);
            node = node->first_child;
        }
        pop(&S, &data);
        node = (struct Tree *)data;

        if (!node->isVisited) {
            if (node->next_sibling) {
                node->isVisited = TRUE;
                push(&S, (element_type)node);
                node = node->next_sibling;
                continue;
            }
        }

        printf("%c ", node->data);
        node = NULL;
    }


    printf("\n");
    return SUCCESS;
}

STATUS levelTraverseTree(struct Tree *T)
{
    element_type data;
    struct Tree *node;
    struct Queue *Q;

    initQueue(&Q);
    put(&Q, (element_type)T);

    printf("level   order: ");
    while (!queueIsEmpty(Q)) {
        get(&Q, &data);
        node = (struct Tree *)data;
        printf("%c ", node->data);

        if (node->first_child)
            put(&Q, (element_type)node->first_child);
        if (node->next_sibling)
            put(&Q, (element_type)node->next_sibling);
    }

    printf("\n");
    return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
STATUS constructTree(struct Tree **T, char *str)
{
    struct Tree *node = NULL;
    struct Stack *S;
    element_type data;
    int num;

    initStack(&S);

    while (*str) {
        if (node == NULL) {
            node = mallocTreeNode();
        }

        if (isspace(*str)) {
            ++str;
            continue;
        } else if (isdigit(*str)) {
            //	num = strtol(str, &str, 10);
            num = *str++;
            initTreeNode(node, (element_type)num);
        } else if (isOperator(*str)) {
            initTreeNode(node, (element_type)*str++);

            pop(&S, &data);
            node->next_sibling = (struct Tree*)data;
            pop(&S, &data);
            node->first_child = (struct Tree*)data;
        } else {
            printf("unknown error: %c \n", *str);
            break;
        }

        data = (element_type)node;
        push(&S, data);
        node = NULL;
    }

    pop(&S, &data);
    *T = (struct Tree *)data;
    assert(S == NULL);

    return SUCCESS;
}

int TEST_tree()
{
    char *expression = "3*4+1";
    char post_string[100] = "";
    struct Tree *T;

    infix2postfix(expression, post_string);
    printf("infix string:   %s\n", expression);
    printf("postfix string: %s\n", post_string);

    initTree(&T);
    constructTree(&T, post_string);

    preTraverseTree(T);
    inTraverseTree(T);
    postTraverseTree(T);
    levelTraverseTree(T);

    return 0;
}

//////////////////////////////////////////////////
CU_TestInfo TEST_SUITE_tree[] = {
    TEST_CASE_ENTRY(TEST_tree),
    CU_TEST_INFO_NULL
};
