#include <utils/utils_main/utils_main.h>
#include <random>
#include <algorithm>
template <typename T>
void mergesort_v1_core(T* data, T* buf, int l, int r)
{
    if (l < r) {
        int m = (l + r) / 2;
        
        mergesort_v1_core(data, buf, l, m);
        mergesort_v1_core(data, buf, m+1, r);
        for (int i = l; i <= m; ++i) {
            buf[i] = data[i];
        }

        for (int i = m+1; i <= r; ++i) {
            buf[m+1+r-i] = data[i];
        }

        int rr = r;
        for (int i = l; i <= rr; ++i) {
            data[i] = buf[l] < buf[r] ? buf[l++] : buf[r--];
        }
    }
}


template <typename T>
void mergesort_v1(T* data, int size)
{
    T* buf = new T[size];

    mergesort_v1_core(data, buf, 0, size-1);

    delete [] buf;
}

template <typename T>
void mergesort_v2(T* data, int size)
{
    T* buf = new T[size];

    for (int step = 1; step <= size; step *= 2) {
        for (int todo = 0; todo < size;) {
            int a = todo;
            int l = step;
            if (a+ l >= size) {
                l = size - todo;
            }
            todo += l;

            for (int i = 0; i < l; ++i) {
                buf[a + i] = data[a+i];
            }

            int b = todo;
            int r = step;
            if (b+ r >= size) {
                r = size - todo;
            }
            int b2 = b;
            b = r > 0 ? b+r-1 : b;
            todo += r;
            for (int i = 0; i < r; ++i) {
                buf[b-i] = data[b2+i];
            }

            if (l > 0 && r > 0) {
                int e = a + l + r;
                for (int i = a; i < e; ++i) {
                    data[i] = buf[a] < buf[b] ? buf[a++] : buf[b--];
                }
            } else if ( l > 0) {
                for (int i = a; i < a+l; ++i) {
                    data[i] = buf[i];
                }
            } else if (r > 0) {
                for (int i = b; i >= b2; --i) {
                    data[b2+b-i] = buf[i];
                }
            }
        }
    }


    delete [] buf;
}

int test_mergesort()
{
    printf("merge sort:\n");
    
    const int size = 10;
    int test_data[size] {17, 3, 11, 17, 6, 2, 18, 1, 4, 12};
    std::default_random_engine random_e;
    random_e.seed(time(nullptr));
    std::uniform_int_distribution<unsigned> u(0, 20);
    for (size_t i = 0; i < size; ++i) {
        //test_data[i] = u(random_e);
    }

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

#if 0
    printf("mergesort_v1:\n");
    mergesort_v1(test_data, size);
#else
    printf("mergesort_v2:\n");
    mergesort_v2(test_data, size);
#endif

    std::for_each(std::begin(test_data), std::end(test_data), [](int& d) {
        printf("%d ", d);
    });
    printf("\n");

    return 0;
}

AUTO_RUN(test_mergesort)
