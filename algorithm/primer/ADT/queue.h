#ifndef QUEUE_H
#define QUEUE_H

namespace alg {
int init_queue();
bool queue_empty();
int queue_size();
bool get(long *v);
bool put(long v);
void print_queue();


int test_queue();

}

#endif // QUEUE_H
