#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>
#include <limits.h>
#include "heapsort.h"

#define MAX_HEAP_SIZE     100
static int heap[MAX_HEAP_SIZE];
static int h_index;
static int h_size;


void upheap(int k)
{
    int v;

    v = heap[k];
    heap[0] = INT_MAX;
    while (heap[k/2] < v) {
        heap[k] = heap[k/2];
        k /= 2;
    }
    heap[k] = v;
}

void downheap(int k)
{
    int v;
    int j;
    v = heap[k];

    while (k <= h_size/2) {
        j = 2 * k;
        if (j < h_size && heap[j] < heap[j+1]) ++j;
        if (v >= heap[j]) break;
        heap[k] = heap[j]; k = j;
    }
    heap[k] = v;
}

void heap_insert(int k)
{
    heap[++h_index] = k;
    upheap(h_index);
}

int heap_remove()
{
    if (h_size == 0) return -1;

    int v = heap[1];
    heap[1] = heap[h_size--];
    downheap(1);

    return v;
}

int heap_replace(int v)
{
    heap[0] = v;
    downheap(0);

    return heap[0];
}


//////////////////////////////////////////////////
//heap root是1，从数组实际取数据的时候减1即可。
void heap_sort_downheap(int a[], int N, int k)
{
    int j;
    int v;

    v = a[k-1];
    while (k <= N/2) {
        j = 2 * k;
        if (j < N && a[j-1] < a[j]) ++j;
        if (v >= a[j-1]) break;
        a[k-1] = a[j-1]; k = j;
    }
    a[k-1] = v;
}

void heap_sort(int a[], int N)
{
    int k, t;

    for (k = N/2; k >= 1; --k)
        heap_sort_downheap(a, N, k);

    while (N > 1) {
        t = a[0]; a[0] = a[N-1]; a[N-1] = t;
        heap_sort_downheap(a, --N, 1);
    }
}

void test_heap_sort()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("heap_sort:\n");
    heap_sort(data, sizeof(data)/sizeof(*data));

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

    heap_sort(test_data, size);

    benchmark_end(&b, "heap_sort");

    print_array(test_data, 100);

    free(test_data);
}













