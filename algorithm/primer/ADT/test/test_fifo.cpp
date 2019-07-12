#include "../fifo.h"
#include <utils/utils_main/utils_main.h>
#include <assert.h>
#include <thread>

int test_fifo()
{
    char test_array[100];
    char buffer[100];

    for (size_t i = 0; i < 100; ++i) {
        test_array[i] = i;
    }

    fifo f(16);

    std::thread t([](fifo* f) {
        char buffer[10];
        while (1) {
            for (size_t i = 0; i < 10; ++i) {
                buffer[i] = i;
            }

            int ret = f->put(buffer, 10);
            if (ret > 0) {
                printf("[put] %d bytes data\n", ret);
            }

            std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
        }
    }, &f);

    int ret = 0;
    while (1) {
        ret = f.get(buffer, 16);
        if (ret > 0) {
            printf("read %d bytes data\n", ret);
        }

        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
    }

    t.join();

    printf("test_fifo finished!");

    return 0;
}

AUTO_RUN(test_fifo);

