#include <utils.h>

#define QUEUE_SIZE 16
static int s_queue[QUEUE_SIZE];

static int tail, head;

int init_queue()
{
    int i;

    for (i = 0; i < QUEUE_SIZE; ++i)
        s_queue[i] = 0;

    tail = 0;
    head = 0;

    return 0;
}

bool queue_empty()
{
    return tail == head;
}

int queue_size()
{
    return tail - head;
}

bool get(int *v)
{
    if (queue_empty()) {
        return false;
    }

    int index = head & QUEUE_SIZE-1;
    ++head;
    *v = s_queue[index];

    return true;
}

bool put(int v)
{
    if (tail - head >= QUEUE_SIZE)
        return false;

    int index = tail & QUEUE_SIZE-1;
    ++tail;
    s_queue[index] = v;

    return true;
}

void print_queue()
{
    int i;

    printf("queue dump:\n");
    for (i = 0; i < queue_size(); ++i) {
        printf("%3d ", s_queue[head+i]);
        if (i && (i+1)%8==0)
            printf("\n");
    }
    printf("\n");
}


int test_queue()
{
    int test_data[100];
    int i;

    init_queue();
    for (i = 0; i < QUEUE_SIZE; ++i) {
        test_data[i] = i+1;

        if (!put(test_data[i])) {
            break;
        }
    }

  //  assert(i == QUEUE_SIZE-1);

    print_queue();

    int dummy;

    for (;;) {
        if (get(&dummy)) {
            ++i;
            printf("get data: %d\n", dummy);
        } else {
            break;
        }

        if (i&1) {
            put(100+i);
        }
    }

    printf("head: %d, tail:%d\n", head, tail);









    return 0;
}
