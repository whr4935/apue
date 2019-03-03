#include <includes.h>
#include "mergesort.h"





void merge_sort_core(int a[], int b[], int l, int r)
{
    int i, j, m;
    int k;

    m = (l + r) / 2;

    if (r > l) {
        merge_sort_core(a, b, l, m);
        merge_sort_core(a, b, m+1, r);

        for (i = m; i >= l; --i) b[i] = a[i];
        for (j = m+1; j <= r; ++j) b[j] = a[r+m-j+1];
        i++;
        j--;
        for (k = l; k <= r; ++k) {
            if (b[i] < b[j])
                a[k] = b[i++];
            else
                a[k] = b[j--];
        }
    }
}


void merge_sort(int a[], int N)
{
    int *b = calloc(sizeof(int), N);

    merge_sort_core(a, b, 0, N-1);

    if (b) {
        free(b);
        b = NULL;
    }
}


void test_merge_sort()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("merge_sort:\n");
    merge_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    /////////////
    int size = 1024 *1024 * 10;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);

    merge_sort(test_data, size);

    benchmark_end(&b, "merge_sort");

    print_array(test_data, 100);

    free(test_data);
}








