#include "misc.h"

static int this_i;
static int this_j;

int ALG1_subSequence(int a[], int n)
{
    int this_sum, max_sum;
    int i, j, k;

    max_sum = 0;

    for (i = 0; i < n; ++i) {
        for (j = i; j < n; ++j) {
            this_sum = 0;

            for (k = i; k <= j; ++k) {
                this_sum += a[k];
                if (this_sum > max_sum) {
                    max_sum = this_sum;
                    this_i = i;
                    this_j = j;
                }
            }
        }
    }

    return max_sum;
}

int ALG2_subSequence(int a[], int n)
{
    int i, j;
    int max_sum, this_sum;

    max_sum = 0;

    for (i = 0; i < n; ++i) {
        this_sum = 0;

        for (j = i; j < n; ++j) {
            this_sum += a[j];
            if (this_sum > max_sum) {
                max_sum = this_sum;
                this_i = i;
                this_j = j;
            }
        }
    }

    return max_sum;
}

int max3(int a, int b, int c)
{
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

int sub_sum_sequence(int a[], int b, int e)
{
    int max_left_sum;
    int max_right_sum;
    int middle;
    int i;
    int max_left_border_sum;
    int max_right_border_sum;
    int this_sum;

    if (b == e) {
//      if (a[b] > 0) {
//          return a[b];
//      } else {
//          return 0;
//      }

        return a[b];
    }

    middle = (b + e) / 2;
    max_left_sum = sub_sum_sequence(a, b, middle);
    max_right_sum = sub_sum_sequence(a, middle + 1, e);

    for (this_sum = 0, max_left_border_sum = 0, i = middle; i >= b; --i) {
        this_sum += a[i];
        if (this_sum > max_left_border_sum) {
            max_left_border_sum = this_sum;
        }
    }

    for (this_sum = 0, max_right_border_sum = 0, i = middle + 1; i <= e; ++i) {
        this_sum += a[i];
        if (this_sum > max_right_border_sum) {
            max_right_border_sum = this_sum;
        }
    }

    return max3(max_left_sum, max_right_sum, max_left_border_sum + max_right_border_sum);
}

int ALG3_subSequence(int a[], int n)
{
    return sub_sum_sequence(a, 0, n - 1);
}

int ALG4_subSequence(int a[], int n)
{
    int i, j;
    int this_sum = 0;
    int max_sum = 0;
    i = 0;

    for (j = 0; j < n; ++j) {
        this_sum += a[j];

        if (this_sum > max_sum) {
            max_sum = this_sum;
            this_i = i;
            this_j = j;
        }

        if (this_sum < 0) {
            this_sum = 0;
            i = j + 1;
        }
    }

    return max_sum;
}


int TEST_subsequence()
{
    int test_data[] = { 10, -2, 15, -9, 1, 4 };
    int max_subSum = 0;

    max_subSum = ALG1_subSequence(test_data, ARRAY_SIZE(test_data));
    printf("ALG 1  max_subSum is %d, subsequence: [ %d --> %d ]\n", max_subSum, this_i, this_j);


    max_subSum = ALG2_subSequence(test_data, ARRAY_SIZE(test_data));
    printf("ALG 2  max_subSum is %d, subsequence: [ %d --> %d ]\n", max_subSum, this_i, this_j);

    max_subSum = ALG3_subSequence(test_data, ARRAY_SIZE(test_data));
    printf("ALG 3  max_subSum is %d.\n", max_subSum);

    max_subSum = ALG4_subSequence(test_data, ARRAY_SIZE(test_data));
    printf("ALG 4  max_subSum is %d, subsequence: [ %d --> %d ]\n", max_subSum, this_i, this_j);

    return 0;
}
