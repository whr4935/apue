#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

struct A {
    A() {
        std::cout << "A()" << std::endl;
    }

    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

A a;

int main(int argc, char *argv[])
{
#if 1
    signal(SIGINT, [](int sig) {
        write(STDERR_FILENO, "exit!\n", 6);
        _exit(0);
    });
#endif

    while (1) {
        sleep(1);
    } 
    return 0;
}

