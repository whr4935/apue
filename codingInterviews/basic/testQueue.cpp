/*************************************************************************
 > File Name: testQueue.cpp
 > Created Time: 2018年04月08日 星期日 16时10分13秒
 ************************************************************************/

#include <iostream>
#include <Utilities/mqueue.h>
#include <thread>
#include <chrono>


int main(int argc, char** argv)
{
    Queue<int> q(15);

    std::thread *th1  = new std::thread([&q] {
        int cnt = 0;
        while (cnt < 100) {
            q.put(cnt++);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread *th2 = new std::thread([&q] {
        for (;;) {
            try {
                std::cout << "th2: " << q.get() << std::endl;
            } catch (std::exception &e) {
                std::cout << e.what() << " ,exit..." << std::endl;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }
    });

    th1->join();
    th2->join();

    delete th1;
    delete th2;
    
    return 0;
}
