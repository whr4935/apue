#include "sort.h"
#include <ADT/stack.h>

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
struct PT_ARG {
    element_type *data;
    int size;
};

struct PT_ARG_3 {
    element_type *data;
    int size;
    int v;
};

//////////////////////////////////////////////////////////////////////////
static element_type test_data[DATA_SIZE];
static element_type test_data_dup[DATA_SIZE];

//////////////////////////////////////////////////////////////////////////
int insert_sort(element_type *e, int len)
{
    int i;
    int j;
    int key;

    for (i = 1; i < len; ++i) {
        key = e[i];

        j = i - 1;
        while (j >= 0 && e[j] > key) {
            e[j + 1] = e[j];
            --j;
        }

        if (j != i - 1)
            e[j + 1] = key;
    }

    return 0;
}

int pt_insert_sort(void *arg)
{
    int ret = -1;
    struct PT_ARG *p = (struct PT_ARG*)arg;

    if (p) {
        ret = insert_sort(p->data, p->size);
    }

    return ret;
}

int test_insert_sort()
{
    int i;
    struct PT_ARG arg;
    int(*pf)(void*) = pt_insert_sort;
    int ret = -1;

    log("before insert sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    arg.data = test_data;
    arg.size = ARRAY_SIZE(test_data);
    ret = statisticTimeFunc(pt_insert_sort, &arg, "insert sort");

    log("after insert sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
int binary_insert(element_type *e, int size, element_type v)
{
    int i;
    element_type beg = 0;
    element_type end = size - 1;
    element_type mid = (beg + end) / 2;

    element_type f_beg = -1;
    element_type f_end = -1;
    BOOL b_insert = FALSE;
    int insert_point = -1;

    if (v > e[end])
        return -1;

    while (beg <= end) {
        if (v > e[end]) {
            beg = end + 1;
            end = size - 1;
        } else if (v <= e[beg]) {
            insert_point = beg;
            break;
        } else {
            end = (beg + end) / 2;
        }
    }

    if (insert_point >= 0) {
        for (i = size - 1; i >= insert_point; --i) {
            e[i + 1] = e[i];
        }
        e[insert_point] = v;
    }

    return 0;
}

int insert_sort_2(element_type *e, int len)
{
    int i;
    int j;
    element_type key;

    for (i = 1; i < len; ++i) {
        key = e[i];

        binary_insert(e, i, key);
    }

    return 0;
}

int pt_insert_sort_2(void *arg)
{
    int ret = -1;
    struct PT_ARG *p = (struct PT_ARG*)arg;

    if (p) {
        ret = insert_sort_2(p->data, p->size);
    }

    return ret;
}

int test_insert_sort_2()
{
    int i;
    struct PT_ARG arg;
    int(*pf)(void*) = pt_insert_sort_2;
    int ret = -1;

    log("before insert sort 2:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    arg.data = test_data;
    arg.size = ARRAY_SIZE(test_data);
    ret = statisticTimeFunc(pf, &arg, "insert sort 2");

    log("after insert sort 2:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
int select_sort(element_type *e, int size)
{
    int i;
    int j;
    int idx;
    element_type tmp;

    for (i = 0; i < size - 1; ++i) {
        for (idx = i, j = i + 1; j < size; ++j) {
            if (e[idx] > e[j]) {
                idx = j;
            }
        }

        if (idx != i) {
            tmp = e[i];
            e[i] = e[idx];
            e[idx] = tmp;
        }
    }

    return 0;
}

int pt_select_sort(void *arg)
{
    int ret = -1;
    struct PT_ARG *p = (struct PT_ARG*)arg;

    if (p) {
        ret = select_sort(p->data, p->size);
    }

    return ret;
}

int test_select_sort()
{
    struct PT_ARG arg;
    int ret = -1;

    log("before select sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    arg.data = test_data;
    arg.size = ARRAY_SIZE(test_data);
    ret = statisticTimeFunc(pt_select_sort, &arg, "select sort");

    log("after select sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
int bubble_sort(element_type *e, int size)
{
    int i;
    int j;
    element_type tmp;

    for (i = size - 1; i > 1; --i) {
        for (j = 0; j < i; ++j) {
            if (e[j] > e[j + 1]) {
                tmp = e[j + 1];
                e[j + 1] = e[j];
                e[j] = tmp;
            }
        }
    }

    return 0;
}

int pt_bubble_sort(void *arg)
{
    int ret = -1;
    struct PT_ARG *p = (struct PT_ARG*)arg;

    if (p) {
        ret = bubble_sort(p->data, p->size);
    }

    return ret;
}

int test_bubble_sort()
{
    struct PT_ARG arg;
    int ret = -1;

    log("before bubble sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    arg.data = test_data;
    arg.size = ARRAY_SIZE(test_data);
    ret = statisticTimeFunc(pt_bubble_sort, &arg, "bubble sort");

    log("after bubble sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
static element_type el[DATA_SIZE], er[DATA_SIZE];

int merge_routine(element_type *e, int p, int q, int r)
{
    int i, j, k;
    int n1, n2;

    n1 = q - p + 1;
    n2 = r - q;

    for (i = 0; i < n1; ++i) {
        el[i] = e[p + i];
    }

    for (i = 0; i < n2; ++i) {
        er[i] = e[q + i + 1];
    }

    i = 0;
    j = 0;
    for (k = p; k <= r; ++k) {
        if (i < n1 && el[i] <= er[j]) {
            e[k] = el[i++];
        } else if (j < n2 && el[i] > er[j]) {
            e[k] = er[j++];
        } else {
            if (i < n1) {
                e[k] = el[i++];
            } else if (j < n2) {
                e[k] = er[j++];
            }
        }
    }

    return 0;
}

int merge_sort_func(element_type *e, int p, int r)
{
    int q = (p + r) / 2;

    if (p < r) {
        merge_sort_func(e, p, q);
        merge_sort_func(e, q + 1, r);
        merge_routine(e, p, q, r);
    }

    return 0;
}

int merge_sort(element_type *e, int size)
{
    merge_sort_func(e, 0, size - 1);

    return 0;
}

int pt_merge_sort(void *arg)
{
    int ret = -1;
    struct PT_ARG *p = (struct PT_ARG*)arg;

    if (p) {
        ret = merge_sort(p->data, p->size);
    }

    return ret;
}

int test_merge_sort()
{
    struct PT_ARG arg;
    int ret = -1;

    log("before merge sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    arg.data = test_data;
    arg.size = ARRAY_SIZE(test_data);
    ret = statisticTimeFunc(pt_merge_sort, &arg, "merge sort");

    log("after merge sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
int find_two_sum_equal_number(element_type *e, int size, int v)
{
    int i, j;
    int tmp;
    int tmp_a = -1;
    int tmp_b = -1;
    int equal_count = 0;

    merge_sort(e, size);
    printArry(test_data, DATA_SIZE);

    i = 0;
    j = size - 1;
    while (i < j) {
        tmp = e[i] + e[j];

        if (tmp == v) {
            printf("YES! %d + %d = %d\n", e[i], e[j], v);
            ++equal_count;
            tmp_a = e[i];
            tmp_b = e[j];
            while (e[++i] == tmp_a)
                ;
            while (e[--j] == tmp_b)
                ;

        } else if (tmp > v) {
            --j;
        } else if (tmp < v) {
            ++i;
        }
    }

    if (tmp_a < 0 || tmp_b < 0)
        printf("NO!\n");
    else {
        printf("count: %d\n", equal_count);
    }

    return 0;
}

int pt_find_two_sum_equal_number(struct PT_ARG_3 *arg)
{
    find_two_sum_equal_number(arg->data, arg->size, arg->v);

    return 0;
}

int test_find_two_sum_equal_number()
{
    struct PT_ARG_3 arg3;
    arg3.data = test_data;
    arg3.size = DATA_SIZE;
    arg3.v = 160;

    statisticTimeFunc(&pt_find_two_sum_equal_number, &arg3, "calc sum: %d", arg3.v);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
int shell_sort(element_type *e, int size)
{
    int i, j, h;
    element_type key;

    h = 1;
    for (h = 1; h < size / 9; h = 3 * h + 1)
        ;

    for (; h > 0; h /= 3) {
        for (i = h; i < size; ++i) {
            key = e[i];
            j = i;

            while (j >= h && e[j - h] > key) {
                e[j] = e[j - h];
                j -= h;
            }

            if (j != i)
                e[j] = key;
        }
    }

    return 0;
}

int pt_shell_sort(struct PT_ARG *arg)
{
    return shell_sort(arg->data, arg->size);
}

int test_shell_sort()
{
    void(*pf)(void*);
    struct PT_ARG arg;
    int ret = -1;

    log("before shell sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    pf = pt_shell_sort;
    arg.data = test_data;
    arg.size = DATA_SIZE;

    ret = statisticTimeFunc(pf, &arg, "shell sort");

    log("after shell sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
element_type quick_sort_get_pivot(element_type *e, int l, int r)
{
    element_type v;
    int m;

    m = l + (r - l) >> 1;

    if (e[l] > e[m])
        swap(e[l], e[m]);
    if (e[l] > e[r])
        swap(e[l], e[r]);
    if (e[m] > e[l])
        swap(e[r], e[m]);

    return e[r];
}

int quick_sort_partition(element_type *e, int l, int r)
{
    element_type v, t;
    int i, j;

    v = e[r];
    i = l - 1;
    j = r;

    for (;;) {
        while (e[++i] < v)
            ;
        while (e[--j] > v) {
            if (j == l)
                break;
        }
        if (i >= j)
            break;
        swap(e[i], e[j]);
    }
    swap(e[i], e[r]);

    return i;
}

int quick_sort_routine(element_type *e, int l, int r)
{
    int i;

    if (l < r) {
        i = quick_sort_partition(e, l, r);
        quick_sort_routine(e, l, i - 1);
        quick_sort_routine(e, i + 1, r);
    }

    return 0;
}

int quick_sort_routine2(element_type *e, int l, int r)
{
    int i;
    struct Stack *S;

    initStack(&S);

    for (;;) {
        while (l < r) {
            i = quick_sort_partition(e, l, r);
            if (i - l > r - i) {
                push(&S, i - 1);
                push(&S, l);
                l = i + 1;
            } else {
                push(&S, r);
                push(&S, i + 1);
                r = i - 1;
            }

            if (r - l < 100) {
                shell_sort(&e[l], r - l + 1);
                break;
            }
        }

        if (stackIsEmpty(S))
            break;

        pop(&S, &l);
        pop(&S, &r);
    }

    return 0;
}

int quick_sort(element_type *e, int size)
{
    quick_sort_routine2(e, 0, size - 1);

    return 0;
}


int pt_quick_sort(struct PT_ARG *arg)
{
    return quick_sort(arg->data, arg->size);
}

int test_quick_sort()
{
    void(*pf)(void*);
    struct PT_ARG arg;
    int ret = -1;

    log("before quick sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    pf = pt_quick_sort;
    arg.data = test_data;
    arg.size = DATA_SIZE;

    ret = statisticTimeFunc(pf, &arg, "quick sort");

    log("after quick sort:\n");
    printArry(test_data, ARRAY_SIZE(test_data));

    return ret;
}

//////////////////////////////////////////////////////////////////////////
static int recursive_count = 0;

int quick_select(element_type *e, int l, int r, int k)
{
    int i;
    int ret = -1;

    ++recursive_count;

    if (l < r) {
        i = quick_sort_partition(e, l, r);
        if (i > k)
            ret = quick_select(e, l, i - 1, k);
        else if (i < k)
            ret = quick_select(e, i + 1, r, k);
        else
            ret = i;
    } else if (l == r) {
        assert(l == k);
        ret = k;
    }

    return ret;
}

int quick_select_no_recursive(element_type *e, int l, int r, int k)
{
    int i;
    int ret = -1;

    while (r > l) {
        i = quick_sort_partition(e, l, r);
        if (i > k)
            r = i - 1;
        else if (i < k)
            l = i + 1;
        else {
            ret = i;
            break;
        }
    }

    if (r == l) {
        assert(r == k);
        ret = k;
    }

    return ret;
}

int pt_quick_select(struct PT_ARG_3 *arg)
{
//	return quick_select(arg->data, 0, arg->size - 1, arg->v);

    return quick_select_no_recursive(arg->data, 0, arg->size - 1, arg->v);
}

int test_quick_select(int v)
{
    void(*pf)(void*);
    struct PT_ARG_3 arg;
    int ret = -1;

    pf = pt_quick_select;
    arg.data = test_data_dup;
    arg.size = DATA_SIZE;
    arg.v = v - 1;

    log("before quick select:\n");
    printArry(arg.data, arg.size);

    ret = statisticTimeFunc(pf, &arg, "quick select");

    log("after quick select:\n");
    printArry(arg.data, arg.size);
    if (ret >= 0)
        printf("\nquick select: %dth, value = %d\n", ret+1, arg.data[ret]);
    else
        printf("\nquick select failed!\n");

    printf ("recursive count: %d\n\n", recursive_count);

    return ret;
}

//////////////////////////////////////////////////////////////////////////
 unsigned int bits(unsigned int d, int k, int j)
{
    return d >> k & ~(~0<<j);
}

int radix_sort_routine(element_type *e, int l, int r, int b)
{
    int i, j;

    if (r > l && b >= 0) {
        i = l, j = r;
        while (j != i) {
            while (bits(e[i], b, 1) == 0 && i < j)
                ++i;
            while (bits(e[j], b, 1) == 1 && j > i)
                --j;
            swap(e[i], e[j]);
        }
        if (bits(e[r], b, 1) == 0)
            ++j;

        radix_sort_routine(e, l, j - 1, b - 1);
        radix_sort_routine(e, j, r, b - 1);
    }

    return 0;
}

int radix_sort(element_type *e, int size)
{
    radix_sort_routine(e, 0, size-1, sizeof(element_type)*8 - 2);

    return 0;
}

int pt_radix_sort(struct PT_ARG *arg)
{
    return radix_sort(arg->data, arg->size);
}

int test_radix_sort()
{
    int(*pf)(void*);
    struct PT_ARG arg;
    int ret;

    pf = pt_radix_sort;
    arg.data = test_data;
    arg.size = DATA_SIZE;

    log("before radix sort:\n");
    printArry(arg.data, arg.size);

    ret = statisticTimeFunc(pf, &arg, "radix sort");

    log("after radix sort:\n");
    printArry(arg.data, arg.size);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
static element_type heap_buf[DATA_SIZE+1];
static int			heap_index;

void upheap(int k)
{
    element_type v;

    v = heap_buf[k];
    heap_buf[0] = 1<<32 -1 /* INT_MAX */;

    while (heap_buf[k/2] <= v) {
        heap_buf[k] = heap_buf[k/2];
        k /= 2;
    }
    heap_buf[k] = v;
}

void downheap(int k)
{
    element_type v;
    int j;

    v = heap_buf[k];
    while (k <= heap_index/2) {
        j = k << 1;
        if (j < heap_index && heap_buf[j] < heap_buf[j + 1]) ++j;
        if (v >= heap_buf[j]) break;

        heap_buf[k] = heap_buf[j];
        k = j;

    }
    heap_buf[k] = v;
}

int heap_insert(element_type v)
{
    heap_buf[++heap_index] = v;
    upheap(heap_index);

    return 0;
}

int heap_remove()
{
    element_type v;

    v = heap_buf[1];
    heap_buf[1] = heap_buf[heap_index--];
    downheap(1);

    return v;
}

int heap_replace(element_type v)
{
    heap_buf[0] = v;
    downheap(0);

    return heap_buf[0];
}

int heap_change()
{

    return 0;
}

int heap_delete()
{

    return 0;
}

int heap_join()
{

    return 0;
}

int heap_construct(element_type *e, int size)
{
    int i;

    heap_index = 0;
    heap_buf[0] = 1<<31 -1/* INT_MAX */;

    for (i = 0; i < size; ++i) {
        heap_insert(e[i]);
    }

    return 0;
}

int heap_sort_routine1(element_type *e, int size)
{
    int i;

    heap_construct(e, size);

    for (i = size - 1; i >= 0; --i) {
        e[i] = heap_remove();
    }

    return 0;
}

void downheap2(element_type *e, int size, int k)
{
    element_type v;
    int j;

    v = e[k];
    while (k <= size/2) {
        j = k << 1;
        if (j < size && e[j] < e[j+1]) ++j;
        if (v >= e[j]) break;
        e[k] = e[j];
        k = j;
    }
    e[k] = v;
}

int heap_sort_routine2(element_type *e, int size)
{
    int i;

    for (i = size/2; i >= 0; --i) {
        downheap2(e, size, i);
    }

    while (size > 0) {
        swap(e[0], e[size]);
        downheap2(e, --size, 0);
    }

    return 0;
}

int heap_sort(element_type *e, int size)
{
//	heap_sort_routine1(e, size);
    heap_sort_routine2(e, size-1);

    return 0;
}

int pt_heap_sort(struct PT_ARG *arg)
{
    return heap_sort(arg->data, arg->size);
}

int test_heap_sort()
{
    int(*pf)(void*);
    struct PT_ARG arg;
    int ret;

    pf = pt_heap_sort;
    arg.data = test_data;
    arg.size = DATA_SIZE;

    log("before heap sort:\n");
    printArry(arg.data, arg.size);

    ret = statisticTimeFunc(pf, &arg, "heap sort");

    log("after heap sort:\n");
    printArry(arg.data, arg.size);

    return ret;
}

//////////////////////////////////////////////////////////////////////////
int init_test_data()
{
    int i;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = rand() % 100 + 1;
    }

    return 0;
}

int init_test_data_2()
{
    int i;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = DATA_SIZE-i;
    }

    return 0;
}

int init_test_data_and_dup()
{
    int i;

    for (i = 0; i < ARRAY_SIZE(test_data); ++i) {
        test_data[i] = rand() % 1000 + 1;
        test_data_dup[i] = test_data[i];
    }

    return 0;
}

int TEST_sort()
{
    int v = DATA_SIZE;

    if (v < 500) {
        v = DATA_SIZE / 3;
    } else {
        v = rand() % 500 + 1;
    }
    v = v == 0 ? 12 : v;


    //  printf("\n=============================\n");
    //  init_test_data();
    //  test_select_sort();
    //

//  printf("\n=============================\n");
//  init_test_data();
//  test_insert_sort();

//  printf("\n=============================\n");
//  init_test_data();
//  test_shell_sort();
//
//  printf("\n=============================\n");
//  init_test_data();
//  test_quick_sort();

//  printf("\n=============================\n");
//  init_test_data_and_dup();
//  test_quick_select(v);
//
//  printf("verify result:");
//  quick_sort(test_data, DATA_SIZE);
//  printf("%dth, value = %d\n\n", v, test_data[v-1]);
//  printf("after sort:\n");
//  printArry(test_data, ARRAY_SIZE(test_data));

    //
    //  printf("\n=============================\n");
    //  init_test_data();
    //  test_insert_sort_2();
    //
    //  printf("\n=============================\n");
    //  init_test_data();
    //  test_bubble_sort();
    //
//  printf("\n=============================\n");
//  init_test_data();
//  test_merge_sort();

    //  printf("\n=============================\n");
    //  init_test_data();
    //  test_find_two_sum_equal_number();

//  printf("\n=============================\n");
//  init_test_data();
//  test_radix_sort();

    printf("\n=============================\n");
    init_test_data();
    test_heap_sort();


    return 0;
}

//////////////////////////////////////////////////
CU_TestInfo TEST_SUITE_sort[] = {
    TEST_CASE_ENTRY(TEST_sort),
    CU_TEST_INFO_NULL
};
