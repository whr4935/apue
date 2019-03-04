#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <thread>

pid_t gettid()
{
    return syscall(__NR_gettid);
}

__thread int a = 10;
thread_local int b = 20;

int main(int argc, char *argv[])
{
    a = 20;
    std::thread t([] {
        std::cout << std::this_thread::get_id() << std::endl;

        std::cout << gettid() << std::endl;
        std::cout << getpid() << std::endl;

        std::cout << a <<std::endl;
    });
    t.join();

    return 0;
}
