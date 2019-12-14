/*************************************************************************
 > File Name: bmSearch.cpp
 > Created Time: 2018年04月09日 星期一 09时31分08秒
 ************************************************************************/

#include "bmSearch.h"
#include <string.h>

template <typename T>
inline T max(T a, T b)
{
    return a > b ? a : b;
}

int bmSearch(const char* str, const char* pattern)
{
    int tab[256] = {-1};
    int strLen = strlen(str);
    int len = strlen(pattern);
    int i, j;

    for (i = 0; i < 256; ++i) {
        tab[i] = len;
    }

    for (i = 0; i < len; ++i) {
        tab[(int)pattern[i]] = len - 1 - i;
    }

    for (i=j=len-1; j >= 0; --i, --j) {
        while (str[i] != pattern[j]) {
            i += max(tab[j], len-j);
            if (i >= strLen) return -1;
            j = len -1;
        }
    }

    return i+1;
}


