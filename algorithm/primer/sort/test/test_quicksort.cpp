#include <stdio.h>
#include <utils/utils_main/utils_main.h>
#include <algorithm>
#include <random>
#include <time.h>

template <typename T>
static void quicksort_core(T* d, int l, int r)
{
    if (l >= r)
        return;

    int partition = d[r];
    int b = l-1;
    int e = r;

    for (;;) {
        while (d[++b] < partition) ;
        while (d[--e] > partition) ;
        if (b >= e) break;

        std::swap(d[b], d[e]);
    }
    std::swap(d[b], d[r]);

    quicksort_core(d, l, b-1);
    quicksort_core(d, b+1, r);
}

template <typename T>
static void quicksort(T *d, size_t n)
{
    quicksort_core(d, 0, n-1);
}


int test_quicksort()
{
    printf("quicksort:\n");
    const int size = 20;
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

    printf("quicksort:\n");
    quicksort(test_data, size);

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    return 0;
}


AUTO_RUN(test_quicksort)
