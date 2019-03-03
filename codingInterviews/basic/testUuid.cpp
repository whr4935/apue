/*************************************************************************
 > File Name: testUuid.cpp
 > Created Time: 2018年03月30日 星期五 17时56分20秒
 ************************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <Utilities/uuid.h>

int main(int argc, char** argv)
{
    UUID &u_id_ptr = UUID::instance();
    u_id_ptr.setEpoch(uint64_t(1367505795100));
    u_id_ptr.setMachine(int32_t(100));

    std::thread th1([&] {
        for (int i = 0; i < 10; ++i) {
            std::cout << u_id_ptr.id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });


    std::thread th2([&] {
        for (int i = 0; i < 10; ++i) {
            std::cout << u_id_ptr.id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    th1.join();
    th2.join();

    return 0;
}
