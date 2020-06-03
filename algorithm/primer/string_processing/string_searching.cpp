#include <stdio.h>
#include <assert.h>
#include <string.h>

int kmp_search(char* p, char *a);

int brutesearch(char* p, char *a)
{
    int i, j, M = strlen(p), N = strlen(a);

    for (i = 0, j = 0; j < M && i < N; ++i, ++j) {
        while (a[i] != p[j]) {
            i -= j-1;
            j = 0;
        }
    }

    if (j == M)
        return i - M;
    else
        return -1;
}

int brutesearch_v2(char *p, char *a)
{
    int i, j=0, M = strlen(p), N = strlen(a);

    for (i = M-1,j=M-1; i < N && j>=0; ++i) {
        for (j = M-1; j >= 0; --i,--j) {
            if (a[i] != p[j]) {
                i += M-j-1;
                break;
            }
        }
    }

    if (j < 0)
        return i;
    else
        return -1;
}

void test_brute_force_search()
{
    char* a = "abccccaaffffdd";
    char* p = "bd";

    int ret = -1;

    ret = brutesearch(p, a);

    printf("ret = %d\n", ret);
    return;
}

///////////////////////////////////////////////////////////////////
//KMP search
//找出从pattern开始处与当前不匹配位置前一个位置最大的匹配长度，这样i就
//不用动了，ｊ就等于从头匹配处的下一个字符位置。
void initnext(char* p, int* next)
{
    int i, j, M = strlen(p);

    next[0] = -1;
    next[1] = 0;
    for (i = 1, j = 0; i < M; ++i, ++j, next[i] = j) {
        while (j >= 0 && p[i] != p[j]) {
            j = next[j];
        }
    }
}

int kmp_search(char* p, char *a)
{
    int i, j, M = strlen(p), N = strlen(a);
    int next[20];
    assert(N <= 20);

    initnext(p, next);

    for (i = 0, j = 0; j < M && i < N; ++i, ++j) {
        while (j >= 0 && a[i] != p[j]) {
            j = next[j];
        }
    }

    if (j == M)
        return i - M;
    else
        return -1;
}



int test_kmp()
{
    char* a = "abccccaaffffdd";
    char* p = "aaf";

    int ret = -1;
    int ret2 = -1;
    int ret3 = -1;

    ret2 = brutesearch(p, a);
    ret3 = brutesearch_v2(p, a);
    assert(ret3 == ret2);

    ret = kmp_search(p, a);

    assert(ret2 == ret);
    printf("ret = %d\n", ret);
    return 0;
}

///////////////////////////////////////////////////////////////////
//Boyer-Moore search

int loop_count;

void suffixes(char *p, int M, int *suffix)
{
    suffix[M-1] = M;
    int i;
    int q;

    for (i = M-2; i >= 0; --i) {
        q = i;
        while (q >= 0 && p[q] == p[M-1-i+q])
            --q;
        suffix[i] = i-q;
    }
}

void preBmGs(char *p, int M, int bmGs[])
{
    int i, j, suffix[100];

    suffixes(p, M, suffix);

    for (i = 0; i < M; ++i)
        bmGs[i] = M;        //整个后缀串都匹配不上，说明后缀之前的字符中没有任何一个字符匹配后缀中的字符，可以安全地移动Ｍ个距离

    j = 0;
    for (i = M-1; i >= 0; --i)
        if (suffix[i] == i+1) {     //前缀
            for (; j < M-1-j; ++j) {
                if (bmGs[j] == M)
                    bmGs[j] = M-1-i;
            }
        }

    for (i = 0; i < M-1; ++i) {     //匹配最靠右的子串
        bmGs[M-1-suffix[i]] = M-1-i;
    }
}

int bm_search(char *p, char *a)
{
    int i, j, M = strlen(p), N = strlen(a);
    int right[256];
    int bmGs[100];

    preBmGs(p, M, bmGs);        //初始化好后缀表

    for (i = 0; i < 256; ++i) { //初始化坏字符表
        right[i] = -1;          //减去-1等于+1
    }

    for (i = 0; i < M; ++i) {
        right[p[i]] = i;
    }

    loop_count = 0;
    int skip = 0;
    int good_suffix = 0;
    for (i = 0; i <= N-M; i+=skip, ++loop_count) {
        if (skip) printf("skip:%d\n", skip);
        skip = 0;
        for (j = M-1; j >= 0; --j) {
            if (p[j] != a[i+j]) {
                skip = j - right[a[i+j]];   //彻底移走pattern，或者与pattern中存在的最右边的该字符对齐
                good_suffix = bmGs[j];
         //       if (good_suffix > skip) skip = good_suffix;
                if (skip < 1)
                    skip = 1;


                break;
            }
        }
        if (skip == 0)
            return i;
    }

    return -1;
}

int test_bm_search()
{
    char* a = "abccccaafaaffaafddfffffaagaabbaabb";
    char* p = "aabbaa";

    int ret = -1;
    int ret2 = -1;
    int ret3 = -1;

    ret2 = brutesearch(p, a);
    ret3 = brutesearch_v2(p, a);
    assert(ret3 == ret2);

    ret = bm_search(p, a);

    assert(ret2 == ret);
    printf("ret = %d, loop_count:%d\n", ret, loop_count);
    return 0;
}

///////////////////////////////////////////////////////////////////
//rabin-karp search

#define q 33554393
#define d 32

int rk_search(char *p, char *a)
{
    int i;
    int M = strlen(p);
    int N = strlen(a);
    int dM = 1;
    int h1 = 0;
    int h2 = 0;

    for (i = 1; i < M; ++i) dM = (dM * d) % q;  //提前算好 a^m-1的值

    for (i = 0; i < M; ++i) {
        h1 = (h1*d + p[i]) % q;
        h2 = (h2*d + a[i]) % q;
    }

    for (i = 0; h1 != h2; ++i) {
        h2 = (h2 + d*q - a[i]*dM) % q;  //减去最高位，乘以进度，加上新的最低位
        h2 = (h2*d + a[i+M]) % q;

        if (i > N-M) return -1;
    }

    return i;
}

#undef q
#undef d

int test_rk_search()
{
    char* a = "abccccaafaaffaafddfffffaagaabbaabb";
    char* p = "abbaa";

    int ret = -1;
    int ret2 = -1;
    int ret3 = -1;

    ret2 = brutesearch(p, a);
    ret3 = brutesearch_v2(p, a);
    assert(ret3 == ret2);

    ret = rk_search(p, a);

    assert(ret2 == ret);
    printf("ret = %d\n", ret);
    return 0;
}

///////////////////////////////////////////////////////////////////
void test_string_searching()
{
    test_brute_force_search();

//    test_kmp();

//    test_bm_search();

    //test_rk_search();
}


