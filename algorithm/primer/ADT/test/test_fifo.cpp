#include "../fifo.h"
#include <utils/utils_main/utils_main.h>
#include <assert.h>
#include <thread>
#include <fstream>
#include <iostream>

namespace alg {

struct Dummy
{
    explicit Dummy(int i) {
        std::cout << "dummy ctor " << i << std::endl;
    }

    Dummy(const Dummy& d) {
        std::cout << "dummy copy ctor " <<  std::endl;
    }
};

int test_fifo()
{
    char test_array[100];

    for (size_t i = 0; i < 100; ++i) {
        test_array[i] = i;
    }

    bool put_thread_exit = false;

    fifo f(3077);
    fifo &rf = f;
    printf("f's addr:%p\n", &f);

    Dummy dummy(1);
    printf("dummy origin addr:%p\n", &dummy);

    std::thread t([&put_thread_exit](fifo& f, Dummy d) {
        printf("dummy  addr:%p\n", &d);
        printf("  f_addr:%p\n", &f);
        const char* filepath = "/mnt/f/CloudMusic/Gala - Faraway.mp3";
        std::fstream infile;
        infile.open(filepath, std::ios_base::in);
        if (!infile.is_open()) {
            printf("%s open failed!", filepath);
            put_thread_exit = true;
            return 0;
        }

        char buffer[1000];
        char* p = buffer;
        while (1) {
            infile.read(buffer, 1000);
            int size = infile.gcount();
            if (size <= 0) {
                printf("readsome return %d\n", size);

                if (infile) {
                    printf("infile check bool return true!\n");
                } else {
                    printf("infile check bool return false!\n");
                }

                printf("infile check eof return  %d, good:%d\n", infile.eof(), infile.good());
                break;
            }
            p = buffer;
            while (size) {
                size_t ret = f.put(p, size);
                if (ret > 0) {
                    //printf("[put] %d bytes data\n", ret);
                    size -= ret;
                    p += ret;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }

        put_thread_exit = true;
        printf("[put] thread finished!\n");

    }, std::ref(rf), dummy);

    int ret = 0;
    
    std::fstream outfile;
    outfile.open("test.bin", std::ios_base::out);
    if (!outfile.is_open()) {
        printf("open test.bin failed!\n");
        return -1;
    }

    char buffer[1024];
    while (1) {
        ret = f.get(buffer, 1024);
        if (ret > 0) {
            //printf("read %d bytes data\n", ret);
            outfile.write(buffer, ret);
        } else {
            if (put_thread_exit) {
                printf("get thread break!\n");
                break;
            }
            //std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
        }

    }

    t.join();

    printf("test_fifo finished!");

    return 0;
}
AUTO_RUN(test_fifo);
}
