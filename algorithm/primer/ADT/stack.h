#ifndef STACK_H
#define STACK_H

namespace alg {

int init_stack();
void push(int v);
int pop();
bool stack_empty();



int test_stack();
void infix2postfix(const char* text, char* post_text);


}

#endif // STACK_H
