#include "search.h"

int binary_search(int a[], int n, int x)
{
//	int i;
    int mid = -1;

    int low, high;

    low = 0; high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x == a[mid])
            return mid;
        else if (x < a[mid]) {
            high = mid - 1;
        } else if (x > a[mid]) {
            low = mid + 1;
        }
    }

    return -1;
}

int TEST_search()
{
    int test_array[] = { 1, 3, 4, 5, 7, 8 };
    int x = 3;
    int result;
    result = binary_search(test_array, ARRAY_SIZE(test_array), x);

    printf("search result is %d, value = %d\n", result, x);

    return 0;
}

//////////////////////////////////////////////////
CU_TestInfo TEST_SUITE_search[] = {
    TEST_CASE_ENTRY(TEST_search),
    CU_TEST_INFO_NULL
};
