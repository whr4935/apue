#include <includes.h>

//////////////////////////////////////////////


//////////////////////////////////////////////
void select_sort(int a[], int N)
{
    int i, j;
    int min;

    for (i = 0; i < N-1; ++i) {
        for (min = i, j = i + 1; j < N; ++j) {
            if (a[j] < a[min]) {
                min = j;
            }
        }

        if (min != i) {
            int tmp = a[i];
            a[i] = a[min];
            a[min] = tmp;
        }
    }
}

int test_select_sort()
{
    int data[] = {1, 3, 2, 10, 9, 102, -4, 33, 76, 28, 11, 19, 95, 27};

    printf("select sort:\n");
    select_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    int size = 102 * 1024;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000 + 1;
    }

    BenchMark b;
    benchmark_begin(&b);
    select_sort(test_data, size);
    benchmark_end(&b, "select_sort");

//    print_array(test_data, 100);

    free(test_data);
    return 0;
}

//////////////////////////////////////////////
void bubble_sort(int a[], int N)
{
    int i, j;
    int tmp;

    for (i = N-1; i > 0; --i)
        for (j = 1; j <= i; ++j) {
            if (a[j-1] > a[j]) {
                tmp = a[j-1];
                a[j-1] = a[j];
                a[j] = tmp;
            }
        }
}


void test_bubble_sort()
{
    int data[] = {1, 3, 2, 10, 9, 102, -4, 33, 76, 28, 11, 19, 95, 27};

    printf("bubble sort:\n");
    bubble_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    int size = 102 * 1024;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);
    bubble_sort(test_data, size);
    benchmark_end(&b, "bubble_sort");

//    print_array(test_data, 100);

    free(test_data);
}

//////////////////////////////////////////////
void insert_sort(int a[], int N)
{
    int i;
    int j;
    int v;

    for (i = 1; i < N; ++i) {
        v = a[i];
        j = i;
        while (j > 0 && a[j-1] > v) {
            a[j] = a[j-1];
            --j;
        }
        a[j] = v;
    }
}

void test_insert_sort()
{
    int data[] = {INT_MIN, 3, 2, 10, 9, 102, -4, 33, 76, 28, 11, 19, 95, 27};

    printf("insert sort:\n");
    insert_sort(data+1, sizeof(data)/sizeof(*data) -1);

    print_array(data+1, sizeof(data)/sizeof(*data) - 1);

    int size = 102 * 1024;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 1; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }
    test_data[0] = INT_MIN;

    BenchMark b;
    benchmark_begin(&b);
    insert_sort(test_data+1, size-1);
    benchmark_end(&b, "insert_sort");

//    print_array(test_data+1, 100);

    free(test_data);
}

void insert_index_sort(int a[], int p[], int N)
{
    int i, j, v;

    for (i = 0; i < N; ++i) p[i] = i;
    for (i = 1; i < N; ++i) {
        v = p[i];
        j = i;
        while (j > 0 && a[p[j-1]] > a[v]) {
            p[j] = p[j-1];
            --j;
        }
        p[j] = v;
    }
}

//用数组实现了链表的效果，非常经典
void insitu(int a[], int p[], int N)
{
    int i;
    int j;
    int k;
    int v;

    for (i = 0; i < N; ++i) {
        if (p[i] != i) {
            k = i; v = a[i];
            do {
                j = k;
                a[j] = a[p[j]];
                k = p[j];
                p[j] = j;
            } while (k != i);
            a[j] = v;
        }
    }
}

void test_insert_index_sort()
{
    int data[] = {1, 3, 2, 10, 9, 102, -4, 33, 76, 28, 11, 19, 95, 27};
    int index[50];

    printf("insert index sort:\n");
    insert_index_sort(data, index, sizeof(data)/sizeof(*data));

    print_index_array(data, index, sizeof(data)/sizeof(*data));
    printf("data array:\n");
    print_array(data, ARRAY_SIZE(data));
    insitu(data, index, ARRAY_SIZE(data));
    printf("data array:\n");
    print_array(data, ARRAY_SIZE(data));


}

//////////////////////////////////////////////
void shell_sort(int a[], int N)
{
    int i, j, h, v;

    for (h = 1; h <= N/9; h = 3*h+1);
    for (; h >= 1; h /= 3)
        for (i = h; i < N; ++i) {
            v = a[i];
            j = i;
            while (j >= h && a[j-h] > v) {
                a[j] = a[j-h];
                j -= h;
            }
            a[j] = v;
        }
}

void test_shell_sort()
{
    int data[] = {INT_MIN, 3, 2, 10, 9, 102, -4, 33, 76, 28, 11, 19, 95, 27};

    printf("shell sort:\n");
    shell_sort(data+1, sizeof(data)/sizeof(*data) -1);

    print_array(data+1, sizeof(data)/sizeof(*data) - 1);

    int size = 1024 * 10240;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 1; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }
    test_data[0] = INT_MIN;

    BenchMark b;
    benchmark_begin(&b);
    shell_sort(test_data+1, size-1);
    benchmark_end(&b, "shell_sort");

//    print_array(test_data+1, 100);

    free(test_data);
}

//////////////////////////////////////////////
//待排序的数必须 >= 0
void distribution_count_sort(int a[], int N)
{
    int M = a[0];
    int i;

    for (i = 1; i < N; ++i) {
        if (a[i] > M) M = a[i];
    }

    printf("N = %d, M = %d\n",N, M);
    int* b = (int*)calloc(sizeof(int), M+1 + N);


    for (i = 0; i < N; ++i) {
        b[a[i]]++;
    }

    for (i = 1; i <= M; ++i) {
        b[i] += b[i-1];
    }

    int *buf = b + M + 1;
    for (i = 0; i < N; ++i) {
        buf[--b[a[i]]] = a[i];
    }

    for (i = 0; i < N; ++i) {
        a[i] = buf[i];
    }

    free(b);
}

void test_distribution_count_sort()
{
    int data[] = {1, 3, 2, 10, 9, 102, 33, 76, 28, 11, 19, 95, 27};

    printf("distribution_count sort:\n");
    distribution_count_sort(data, sizeof(data)/sizeof(*data));

    print_array(data, sizeof(data)/sizeof(*data));

    int size = 1024 * 1024 * 10;
    int *test_data = malloc(size * sizeof(int));
    int i;

    srand(time(NULL));
    for (i = 0; i < size; ++i) {
        test_data[i] = rand() % 1000  + 1;
    }

    BenchMark b;
    benchmark_begin(&b);
    distribution_count_sort(test_data, size);
    benchmark_end(&b, "distribution_count_sort");

    print_array(test_data, 100);

    free(test_data);
}


//////////////////////////////////////////////
int test_elementary_sort()
{
//    test_select_sort();

//    test_bubble_sort();

//    test_insert_sort();

//    test_shell_sort();

//    test_insert_index_sort();

//    test_distribution_count_sort();

    return 0;
}








