#include <utils/utils_main/utils_main.h>
#include <stdio.h>
#include <string.h>

static void parse_pattern(const char* p, int* next, int len)
{
    for (int i = 0;i < len; ++i) {
        next[i] = -1;
    }

    next[0] = -1;
    for (int i = 0, j = -1; i < len; ++i, ++j, next[i] = j) {
        while (j >= 0 && p[i] != p[j]) {
            j = next[j];
        }
    }
}

static int kmp_search(const char* a, const char* p)
{
    int a_len = strlen(a);
    int p_len = strlen(p);
    int* next = new int[p_len];
    int ret = -1;

    parse_pattern(p, next, p_len);
    int i = 0;
    int j = 0;
    for (; i < a_len && j < p_len; ++i, ++j) {
        while (j >= 0 && a[i] != p[j]) {
            j = next[j];
        }
    }
    if (j == p_len) {
        ret = i - p_len;
    }

    delete [] next;

    return ret;
}


int test_kmp_search()
{
    const char* a = "abcbababcdefg";
    const char* p = "abab";

    int ret = kmp_search(a, p);
    printf("str = %s\n", a);
    printf("search \"%s\" return %d\n", p, ret);

    return 0;
}

AUTO_RUN(test_kmp_search)
