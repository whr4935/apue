#include <iostream>
#include <ucontext.h>

ucontext_t main_context;
ucontext_t func1_context;
ucontext_t func2_context;

void func1()
{
    std::cout << __func__ << std::endl;

    swapcontext(&func1_context, &main_context);
}


void func2()
{
    std::cout << __func__ << std::endl;
}


#define STACK_SIZE 16384
char func1_stack[STACK_SIZE];
char func2_stack[STACK_SIZE];

int main(int argc, char *argv[])
{
    std::cout << "main begin test" << std::endl;    

    int err=getcontext(&func1_context);
    if (err < 0) {
        perror("getcontext");
    }
    func1_context.uc_stack.ss_sp = func1_stack;
    func1_context.uc_stack.ss_size = STACK_SIZE;
    makecontext(&func1_context, func1, 0);

    err=getcontext(&func2_context);
    if (err < 0) {
        perror("getcontext");
    }
    func2_context.uc_stack.ss_sp = func2_stack;
    func2_context.uc_stack.ss_size = STACK_SIZE;
    makecontext(&func2_context, func2, 0);

    err = swapcontext(&main_context, &func1_context);
    if (err < 0) {
        perror("swapcontext");
    }
    
    std::cout << "main exit" << std::endl;

    return 0;
}
