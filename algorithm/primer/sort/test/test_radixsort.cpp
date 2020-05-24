#include <utils/utils_main/utils_main.h>
#include <random>
#include <algorithm>


template <typename T>
static inline T bits(T d, int p, int m)
{
    return (d>>p) & ~(~0 << m);
}

template <typename T>
void straight_radixsort(T* data, size_t size)
{
    size_t* buf = new size_t[size];
    int count[2] {};
    int w = sizeof(T) * 8;

    for (int i = 0; i < w; ++i) {
        count[0] = count[1] = 0;

        for (int j = 0; j < size; ++j) {
            count[bits(data[j], i, 1)]++;
        }

        count[1] += count[0];

        for (int j = size-1; j >= 0; --j) {
            buf[--count[bits(data[j], i, 1)]] = data[j];
        }

        for (int j = 0; j < size; ++j) {
            data[j] = buf[j];
        }
    }

    delete [] buf;
}


int test_radixsort()
{
    printf("straight radix sort:\n");
    
    const int size = 10;
    int test_data[size];
    std::default_random_engine random_e;
    random_e.seed(time(nullptr));
    std::uniform_int_distribution<unsigned> u(0, 20);
    for (size_t i = 0; i < size; ++i) {
        //test_data[i] = size - i;
        test_data[i] = u(random_e);
    }

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    printf("radixsort:\n");
    straight_radixsort(test_data, size);

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    return 0;
}

AUTO_RUN(test_radixsort)
