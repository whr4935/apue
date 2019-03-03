#include <includes.h>
#include "quicksort.h"

#include "ADT/stack.h"
#include "elementary_sort.h"


void quick_sort_core(int a[], int l, int r)
{
    int v, t;
    int i, j;

    if (r > l) {
        v = a[r];
        i = l-1;
        j = r;
        for (;;) {
            while (a[++i] < v) ;
            while (a[--j] > v) ;            //不能用>=判断，会非常慢

            if (i >= j) break;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
        t = a[i]; a[i] = v; a[r] = t;

        quick_sort_core(a, l, i-1);
        quick_sort_core(a, i+1, r);
    }
}

void quick_sort(int a[], int N)
{
    quick_sort_core(a, 0, N-1);
}

///////////////////////////////////////////////////
//remove recurisve
void quick_sort_v1(int a[], int N)
{
    int l = 0;
    int r = N-1;
    init_stack();
    int v;
    int t;
    int i, j;

    for (;;) {
        if (r > l) {
            i = l-1;
            j = r;
            v = a[r];
            for (;;) {
                while (a[++i] < v) ;
                while (a[--j] > v) ;
                if (i >= j) break;
                t = a[i]; a[i] = a[j]; a[j] = t;
            }
            t = a[i]; a[i] = a[r]; a[r] = t;

            if (i-l > r-i) {
                push(i-1);
                push(l);
                l = i + 1;
            } else {
                push(r);
                push(i+1);
                r = i - 1;
            }
        } else if (stack_empty()) {
            break;
        } else {
            l = pop();
            r = pop();
        }
    }
}


///////////////////////////////////////////////////
//small subfile use insert sort

#define SMALL_LIMIT 30

void quick_sort_v2(int a[], int N)
{
    int l = 0;
    int r = N-1;
    init_stack();
    int v;
    int t;
    int i, j;

    for (;;) {
        if (r - l > SMALL_LIMIT) {
            i = l-1;
            j = r;
            v = a[r];
            for (;;) {
                while (a[++i] < v) ;
                while (a[--j] > v) ;
                if (i >= j) break;
                t = a[i]; a[i] = a[j]; a[j] = t;
            }
            t = a[i]; a[i] = a[r]; a[r] = t;

            if (i-l > r-i) {
                push(i-1);
                push(l);
                l = i + 1;
            } else {
                push(r);
                push(i+1);
                r = i - 1;
            }
        } else if (stack_empty()) {
            break;
        } else {
            l = pop();
            r = pop();
        }
    }

    insert_sort(a, N);
}


///////////////////////////////////////////////////
//median of three partioning

void quick_sort_v3(int a[], int N)
{
    int l = 0;
    int r = N-1;
    init_stack();
    int v;
    int t;
    int i, j;
    int mid;

    for (;;) {
        if (r - l > SMALL_LIMIT) {
            mid = (l + r) >> 1;
            if (a[l] > a[mid]) {
                v = a[l];
                a[l] = a[mid];
                a[mid] = v;
            }
            if (a[l] > a[r]) {
                v = a[l];
                a[l] = a[r];
                a[r] = v;
            }
            if (a[mid] > a[r]) {
                v = a[mid];
                a[mid] = a[r];
                a[r] = v;
            }
            v = a[mid];
            a[mid] = a[r-1];
            a[r-1] = v;

            i = l;
            j = r-1;
            v = a[r-1];
            for (;;) {
                while (a[++i] < v) ;
                while (a[--j] > v) ;
                if (i >= j) break;
                t = a[i]; a[i] = a[j]; a[j] = t;
            }
            t = a[i]; a[i] = a[r]; a[r] = t;

            if (i-l > r-i) {
                push(i-1);
                push(l);
                l = i + 1;
            } else {
                push(r);
                push(i+1);
                r = i - 1;
            }
        } else if (stack_empty()) {
            break;
        } else {
            l = pop();
            r = pop();
        }
    }

    insert_sort(a, N);
}


///////////////////////////////////////////////////
#define quick_sort_func  quick_sort


void test_quick_sort()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("quick sort:\n");
    quick_sort_func(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    /////////////
    int size = 1024 *1024 * 20;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);

    quick_sort_func(test_data, size);

    benchmark_end(&b, "quick sort");

//    print_array(test_data, 100);

    free(test_data);
}

///////////////////////////////////////////////////
//select
int select_kth_core(int a[], int l, int r, int k)
{
    int i, j, v, t;

    if (r > l) {
        i = l - 1;
        j = r;
        v = a[r];
        for (;;) {
            while (a[++i] < v) ;
            while (a[--j] > v) ;
            if (i >= j) break;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
        t = a[i]; a[i] = a[r]; a[r] = t;

        if (k == i)
            return i;
        else if (k > i) {
            return select_kth_core(a, i+1, r, k);
        } else {
            return select_kth_core(a, l, i-1, k);
        }
    }
}

int select_kth_no_recursive(int a[], int l, int r, int k)
{
    int i, j, v, t;

    while (r > l) {
        i = l - 1;
        j = r;
        v = a[r];
        for (;;) {
            while (a[++i] < v) ;
            while (a[--j] > v) ;
            if (i >= j) break;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
        t = a[i]; a[i] = a[r]; a[r] = t;

        if (k == i)
            return i;
        else if (k > i)
            l = i+1;
        else
            r = i-1;
    }
}


int select_kth(int a[], int N, int k)
{
//    return select_kth_core(a, 0, N-1, k);

    return select_kth_no_recursive(a, 0, N-1, k);
}

void test_select_kth()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};
    int i = -1;

    printf("select kth:\n");
    i = select_kth(data, sizeof(data)/sizeof(*data), 5);
    printf("kth:%d, value:%d\n", i, data[i]);


    /////////////
    int size = 1024 *1024 * 20;
    int *test_data = malloc(size * sizeof(int));


    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);

    i = select_kth(test_data, size, 200000);
    printf("kth:%d, value:%d\n", i, test_data[i]);

    benchmark_end(&b, "select kth");

    print_array(test_data, 100);

    free(test_data);
}
















