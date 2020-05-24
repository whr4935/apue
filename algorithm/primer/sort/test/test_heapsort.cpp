#include <utils/utils_main/utils_main.h>
#include <algorithm>
#include <random>
#include <time.h>

//2n+1 2n+2
//0 1 2
//1 3 4
//2 5 6
template <typename T>
static void upheap(T* data, int size, int v)
{
    int j = size;

    int k = (j-1)/2;
    while (k >= 0) {
        if (data[k] <= data[j]) {
            break;
        }

        data[j] = data[k];
        j = k;
        k = (j-1)/2;
    }

    data[j] = v;
}

template <typename T>
static void downheap(T* data, int size, int p)
{
    int v = data[p];
    while (p < size/2) {
        int k = 2*p+1;
        if (k < size) {
            if ( k < size-1 && data[k] < data[k+1]) {
                k++;
            }

            if (v < data[k]) {
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
