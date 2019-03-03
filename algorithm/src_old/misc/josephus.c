#include <algorithm.h>

struct node {
    int key;
    struct node *next;
};

#define N 9
#define M 5

int test_josephus_loop()
{
    struct node *head = NULL;
    struct node *x;

    int i = N;
    while (i--) {
        x = (struct node*)malloc(sizeof(struct node));
        if (x == NULL) {
            printf("malloc x failed!\n");
            return -1;
        }
        x->key = i+1;
        x->next = head;
        head = x;
    }

    struct node **pp = &head;
    while (*pp) {
        pp = &(*pp)->next;
    }
    *pp = head;

    for (x = head; x;) {
        printf("key:%d\n", x->key);
        if (x->next == head)
            break;
        x = x->next;
    }

    /* x point to the last element, kill the Mth element */
    while (x != x->next) {
        for (i = M; --i;) {
            x = x->next;
        }
        printf("%d ", x->next->key);
        free(x->next);
        x->next = x->next->next;
    }
    printf("%d\n", x->key);

    return 0;
}
