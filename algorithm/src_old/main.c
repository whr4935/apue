#include <algorithm.h>
#include "main.h"

//////////////////////////////////////////////////
static CU_SuiteInfo suites[] = {
#if TEST_MISC_EN
    TEST_SUITE_ENTRY(TEST_SUITE_misc),
#endif

#if TEST_LIST_EN
    TEST_SUITE_ENTRY(TEST_SUITE_list),
#endif

#if TEST_STACK_EN
    TEST_SUITE_ENTRY(TEST_SUITE_stack),
#endif

#if TEST_QUEUE_EN
    TEST_SUITE_ENTRY(TEST_SUITE_queue),
#endif

#if TEST_TREE_EN
    TEST_SUITE_ENTRY(TEST_SUITE_tree),
#endif

#if TEST_SORT_EN
    TEST_SUITE_ENTRY(TEST_SUITE_sort),
#endif

#if TEST_SEARCH_EN
    TEST_SUITE_ENTRY(TEST_SUITE_search),
#endif

    CU_SUITE_INFO_NULL
};


//////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int i;

    CU_initialize_registry();
    CU_register_suites(suites);
    initUtil();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    uninitUtil();
#ifdef NDEBUG
    getchar();
#endif

    CU_cleanup_registry();

    return 0;
}
