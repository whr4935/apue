#include <iostream>
#include <thread>
#include <chrono>

thread_local int var = 0;

static int static_test()
{
    std::cout << "var addr:" << &var << std::endl;

    ++var;
    return var;
}


int test_thread_local()
{
    std::cout << static_test() << std::endl;
    std::cout << static_test() << std::endl;
    std::cout << static_test() << std::endl;
    std::cout << "main thread var value:" << var << std::endl;

    uintptr_t  addr = (uintptr_t)&var;

    std::thread t([addr] {
        //thread_local int var = 0;
        std::cout << "main thread var addr:" << std::hex << addr << std::dec << std::endl;
        std::cout << "thread test var addr:" << &var << std::endl;

        while (var < 3) {
            ++var;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << var << std::endl;
        }

        int* pVar = (int*)addr;
        ++*pVar;
        std::cout << __LINE__ << ": main thread var :" << std::hex << *pVar << std::endl;
    });

    t.join();

    std::cout << "concurrency: " << t.hardware_concurrency() << std::endl;
    std::cout << __LINE__ << ": main thread var value:" << var << std::endl;


    return 0;
}
