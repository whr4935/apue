#include <utils/utils_main/utils_main.h>
#include <algorithm>
#include <random>
#include <time.h>

//2n+1
template <typename T>
static void upheap(T* data, int size, int v)
{
    int j = size;
    data[j] = v;

    int k = (j-1)/2;
    while (k >= 0) {
        if (data[k] <= v) {
            break;
        }

        data[j] = data[k];
        j = k;
        if (j > 0) {
            k = (j-1)/2;
        } else {
            break;
        }
    }

    data[j] = v;
}

//2n+1 2n+2
//0 1 2
//1 3 4
//2 5 6
template <typename T>
static void downheap(T* data, int size, int p)
{
    int v = data[p];
    while (p < size/2) {
        int k = 2*p+1;
        if (k < size) {
            if ( k < size-1 && data[k] > data[k+1]) {
                k++;
            }

            if (v > data[k]) {
                data[p] = data[k];
                p = k;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    data[p] = v;
}

template <typename T>
void heapsort(T* data, size_t size)
{
    for (int i = size/2-1; i >= 0; --i) {
        downheap(data, size, i);
    }

    while (size > 1) {
        int v = data[0];
        data[0] = data[size-1];
        data[size-1] = v;
        downheap(data, --size, 0);
    }
}

int test_heapsort()
{
    printf("heapsort:\n");
    const int size = 10;
    int test_data[size] {17, 3, 11, 17, 6, 2, 18, 1, 4, 12};

    std::default_random_engine random_e;
    random_e.seed(time(nullptr));
    std::uniform_int_distribution<unsigned> u(0, 20);
    for (size_t i = 0; i < size; ++i) {
        test_data[i] = u(random_e);
    }

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    printf("heapsort:\n");
    heapsort(test_data, size);

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");
    return 0;
}

AUTO_RUN(test_heapsort)

int test_hep_insert()
{
    std::default_random_engine e;
    //e.seed(time(NULL));
    
    std::uniform_int_distribution<unsigned> u(0, 100);
    size_t N = 10;
    int test_data[10];

    for (size_t i = 0; i < N; ++i) {
        //test_data[i] = N-i;
        test_data[i] = u(e);
    }

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    int heap[10]{0};
    for (size_t i = 0; i < N; ++i) {
        int d = test_data[i];
        upheap(heap, i, d);
    }

    printf("heap index:\n");
    for (size_t i = 0; i < N; ++i) {
        printf("%-2zu ", i);
    }
    printf("\n");
    std::for_each(std::begin(heap), std::end(heap), [](int& d) {
        printf("%-2d ", d);
    });
    printf("\n");
    for (int i = N-1; i >= 0; --i) {
        int d = heap[0];
        printf("%-2d ", d);
        if (i > 0) {
            heap[0] = heap[i];
            downheap(heap, i, 0);
        }
    }
    printf("\n");

    return 0;
}

AUTO_RUN(test_hep_insert)
