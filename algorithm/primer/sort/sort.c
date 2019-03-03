#include <includes.h>

#include "elementary_sort.h"
#include "quicksort.h"
#include "radixsort.h"
#include "heapsort.h"
#include "mergesort.h"


int test_sort(int argc, char **argv)
{
    BEGIN_MODULE;

//    test_elementary_sort();

    test_quick_sort();

//    test_select_kth();

//    test_radix_sort();

//    test_heap_sort();

//    test_merge_sort();


    END_MODULE;
    return 0;
}

