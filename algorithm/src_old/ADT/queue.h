#ifndef _QUEUE_H
#define _QUEUE_H

#include <algorithm.h>

struct Queue {
    element_type data;
    struct Queue *next;
};

//////////////////////////////////////////////////////////////////////////
struct Queue* mallocQueueNode();
STATUS freeQueueNode(struct Queue *node);
STATUS initQueue(struct Queue **Q);
STATUS put(struct Queue **Q, element_type data);
STATUS get(struct Queue **Q, element_type *data);
BOOL queueIsEmpty(struct Queue *Q);
BOOL queueIsFull(struct Queue *Q);

#endif
