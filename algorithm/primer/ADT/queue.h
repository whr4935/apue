#ifndef QUEUE_H
#define QUEUE_H

int init_queue();
bool queue_empty();
int queue_size();
bool get(void *v);
bool put(long v);
void print_queue();


int test_queue();


#endif // QUEUE_H
