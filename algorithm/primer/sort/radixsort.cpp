#include <stdio.h>
#include <utils/utils.h>
#include <math.h>
#include "radixsort.h"




unsigned bits(int x, int k, int j)
{
    return (x>>k) & ~(~0<<j);
}

void radix_exchange_sort_core(int a[], int l, int r, int b)
{
    int i, j, t;

    if (r > l && b >= 0) {
        i = l;
        j = r;
        for (;;) {
            while (bits(a[i], b, 1)==0 && i < j) ++i;
            while (bits(a[j], b, 1)!=0 && j > i) --j;
            if (i == j) break;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
        if (bits(a[r], b, 1) == 0) ++j;
        radix_exchange_sort_core(a, l, j-1, b-1);
        radix_exchange_sort_core(a, j, r, b-1);
    }
}

void radix_exchange_sort(int a[], int N)
{
    return radix_exchange_sort_core(a, 0, N-1, sizeof(int)*8-1);
}


void test_radix_exchange_sort()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("radix_exchange_sort:\n");
    radix_exchange_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    /////////////
    int size = 1024 *1024 * 10;
    int *test_data = (int*)malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);

    radix_exchange_sort(test_data, size);

    benchmark_end(&b, "radix_exchange_sort");

//    print_array(test_data, 100);

    free(test_data);
}



////////////////////////////////////////////////////////////
void straight_radix_sort(int a[], int N)
{
    int m = 8;
    int M = pow(2, m);
    int* b = (int*)calloc(sizeof(int), M);
    int* buf = (int*)calloc(sizeof(int), N);
    int pass;
    int i;
    int w = sizeof(int)*8 / m - 1;

    for (pass = 0; pass < w; ++pass) {
        for (i = 0; i < M; ++i) {
            b[i] = 0;
        }

        for (i = 0; i < N; ++i) {
            b[bits(a[i], pass, m)]++;
        }

        for (i = 1; i < M; ++i) {
            b[i] += b[i-1];
        }

        for (i = N-1; i >= 0; --i) {        //为什么它是稳定的排序？
            buf[--b[bits(a[i], pass, m)]] = a[i];
        }

        for (i = 0; i < N; ++i) {
            a[i] = buf[i];
//            printf("%#x ", a[i]);
        }
//        printf("\n");
    }

    free(buf);
    free(b);
}

void test_straight_radix_sort()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("straight_radix_sort:\n");
    straight_radix_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    /////////////
    int size = 1024 *1024 * 10;
    int *test_data = (int*)malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);

    straight_radix_sort(test_data, size);

    benchmark_end(&b, "straight_radix_sort");

    print_array(test_data, 100);

    free(test_data);
}




////////////////////////////////////////////////////////////
void test_radix_sort()
{
//    test_radix_exchange_sort();

    test_straight_radix_sort();

}













