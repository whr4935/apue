#include "queue.h"

struct Queue* mallocQueueNode()
{
    struct Queue *node;

    node = (struct Queue *)calloc(1, sizeof(struct Queue));

    return node;
}

STATUS freeQueueNode(struct Queue *node)
{
    if (node == NULL)
        return FAILURE;

    free(node);
    return SUCCESS;
}

STATUS initQueueNode(struct Queue *node, element_type data)
{
    node->data = data;
    node->next = NULL;

    return SUCCESS;
}

STATUS initQueue(struct Queue **Q)
{
    *Q = NULL;

    return SUCCESS;
}

STATUS put(struct Queue **Q, element_type data)
{
    struct Queue *node;

    if (queueIsFull(*Q))
        return FAILURE;

    node = mallocQueueNode();
    if (node == NULL)
        return FAILURE;

    initQueueNode(node, data);

    while (*Q) {
        Q = &(*Q)->next;
    }
    *Q = node;

    return SUCCESS;
}

STATUS get(struct Queue **Q, element_type *data)
{
    struct Queue *node;

    if (queueIsEmpty(*Q))
        return FAILURE;

    node = *Q;
    *Q = node->next;
    *data = node->data;

    freeQueueNode(node);

    return SUCCESS;
}

BOOL queueIsEmpty(struct Queue *Q)
{
    return Q == NULL;
}

BOOL queueIsFull(struct Queue *Q)
{
    return FALSE;
}

STATUS printQueue(struct Queue *Q, char *fmt, ...)
{
    va_list ap;
    struct Queue *node;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    node = Q;
    while (node) {
        printf("%d ", (int)node->data);
        node = node->next;
    }
    printf("\n");

    return SUCCESS;
}

int TEST_queue()
{
    struct Queue *Q = NULL;
    element_type test_data[10];
    int i;
    int count = 0;
    int count2 = 0;
    element_type data;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = i + 1;
    }

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        put(&Q, test_data[i]);
    }
    printQueue(Q, "Queue<%d>: ", ++count);

    for (i = 0; i < 5; ++i) {
        get(&Q, &data);
    }
    printQueue(Q, "Queue<%d>: ", ++count);

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        put(&Q, test_data[i] + 10);
    }
    printQueue(Q, "Queue<%d>: ", ++count);

    return 0;
}

CU_TestInfo TEST_SUITE_queue[] = {
    TEST_CASE_ENTRY(TEST_queue),
    CU_TEST_INFO_NULL
};

//////////////////////////////////////////////////////////////////////////
#if 0

#define QUEUE_BUF_SIZE		10

struct queue{
    int front;
    int rear;
    Dataptr buf[QUEUE_BUF_SIZE];
};

struct queue* createQueue()
{
    struct queue* Q;

    Q = (struct queue*)calloc(1, sizeof(struct queue));

    return Q;
}

STATUS destroyQueue(struct queue* Q)
{
    if (Q) {
        free(Q);
        return SUCCESS;
    }

    return FAILURE;
}

STATUS initQueue(struct queue *Q)
{
    int i;

    Q->front = 0;
    Q->rear = 0;
    for (i = 0; i < QUEUE_BUF_SIZE; ++i) {
        Q->buf[i] = 0;
    }

    return SUCCESS;
}

BOOL isEmptyQueue(struct queue *Q)
{
    return Q->front == Q->rear;
}

BOOL isFullQueue(struct queue *Q)
{
    return (Q->rear + 1) % QUEUE_BUF_SIZE == Q->front;
}

int getQueueSize(struct queue* Q)
{
    int size;

    size = (Q->rear + QUEUE_BUF_SIZE - Q->front) % QUEUE_BUF_SIZE;

    return size;
}

STATUS enQueue(struct queue *Q, Dataptr data)
{
    if (isFullQueue(Q))
        return FAILURE;

    Q->buf[Q->rear] = data;
    Q->rear = (Q->rear + 1) % QUEUE_BUF_SIZE;

    return SUCCESS;
}

STATUS deQueue(struct queue *Q, Dataptr *pdata)
{
    if (isEmptyQueue(Q)) {
        pdata = NULL;
        return FAILURE;
    }

    if (pdata) {
        *pdata = Q->buf[Q->front];
    }
    Q->front = (Q->front + 1) % QUEUE_BUF_SIZE;

    return SUCCESS;
}

STATUS printQueue(struct queue *Q)
{
    int i;

    printf("queue size (%d): ", getQueueSize(Q));
    for (i = Q->front; i != Q->rear; ) {
        printf("%d ", Q->buf[i]);

        i = (i + 1) % QUEUE_BUF_SIZE;
    }
    printf("\n");

    return SUCCESS;
}

/*
 * Another queue
*/
struct queue_record {
    unsigned int q_max_size;
    unsigned int q_front;
    unsigned int q_rear;
    unsigned int q_size;

    element_type q_array[];
};

struct queue_record* create_queue(unsigned int max_size)
{
    struct queue_record *Q;
    unsigned int i;

    Q = (struct queue_record *)calloc(1, sizeof(struct queue_record) + max_size * sizeof(element_type));

    Q->q_max_size = max_size;
    for (i = 0; i < max_size; ++i) {
        Q->q_array[i] = (element_type)((unsigned int)~0>>1);
    }

    return Q;
}

STATUS destroy_queue(struct queue* Q)
{
    if (Q) {
        free(Q);
        return SUCCESS;
    }

    return FAILURE;
}

BOOL is_empty(struct queue_record *Q)
{
    return Q->q_size == 0;
}

BOOL is_full(struct queue_record *Q)
{
    return Q->q_size == Q->q_max_size;
};

STATUS make_null(struct queue_record *Q)
{
    Q->q_size = 0;
    Q->q_front = 0;
    Q->q_rear = 0;

    return SUCCESS;
}

unsigned int succ(unsigned int value, struct queue_record *Q)
{
    if (++value == Q->q_max_size)
        value = 0;

    return value;
}

STATUS en_queue(element_type x, struct queue_record *Q)
{
    if (is_full(Q)) {
        return FAILURE;
    } else {
        Q->q_size++;
        Q->q_array[Q->q_rear] = x;
        Q->q_rear = succ(Q->q_rear, Q);


        return SUCCESS;
    }
}

STATUS de_queue(element_type *x, struct queue_record *Q)
{
    if (is_empty(Q)) {
        return FAILURE;
    }

    Q->q_size--;
    if (x)
        *x = Q->q_array[Q->q_front];
    Q->q_front = succ(Q->q_front, Q);

    return SUCCESS;
}

STATUS print_queue(struct queue_record *Q)
{
    int i;
    int idx;

    printf("queue: ");
    for (i = 0; i < Q->q_size; ++i) {
        idx = Q->q_front + i;
        if (idx >= Q->q_max_size)
            idx = 0;

        printf("%d ", Q->q_array[idx]);
    }
    printf("\n");

    return SUCCESS;
}

struct emp_struct {
//  int a;
//  int b;
    int q[];
};

//////////////////////////////////////////////////////////////////////////
int TEST_queue()
{
    struct queue *Q = NULL;
    int i;

    Q = createQueue();

    for (i = 0; i < QUEUE_BUF_SIZE; ++i) {
        enQueue(Q, i + 1);
    }
    deQueue(Q, NULL);
    deQueue(Q, NULL);

    enQueue(Q, 1004);

    printQueue(Q);


    if (destroyQueue(Q) == SUCCESS) {
        Q = NULL;
    }

//////////////////////////////////////////////////////////////////////////
    int s = sizeof(struct emp_struct);

    struct queue_record *Q2;

    Q2 = create_queue(10);
    make_null(Q2);

    for (i = 0; i < 10; ++i) {
        en_queue(i+1, Q2);
    }

//

    de_queue(NULL, Q2);
    en_queue(11, Q2);

    print_queue(Q2);

    return 0;
}
#endif
