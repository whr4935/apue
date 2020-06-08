#include <utils/utils_main/utils_main.h>
#include <string.h>

static void preBc(const char*p, int p_len, int bmBc[])
{
    for (int i = 0; i < 256; ++i) {
        bmBc[i] = p_len;
    }

    for (int i = 0; i < p_len-1; ++i) {
        bmBc[p[i]] = p_len-1 - i;
    }
}

static void suffix(const char* p, int p_len, int suff[])
{
    suff[p_len - 1] = p_len;

    for (int i = p_len-2; i >= 0; --i) {
        int j = i;
        while (j >= 0 && p[j] == p[p_len-1 - (i-j)]) {
            --j;
        }
        suff[i] = i - j;
    }
}

static void preGs(const char* p, int p_len, int bmGs[])
{
    int i, j;
    int suff[p_len];

    suffix(p, p_len, suff);

    for (i = 0; i < p_len; ++i) {
        bmGs[i] = p_len;
    }

    j = 0;
    for (i = p_len-1; i >= 0; --i) {
        if (suff[i] == i+1) { //sub prefix
            for (; j < p_len-1-i; ++j) {
                if (bmGs[j] == p_len) {
                    bmGs[j] = p_len-1-i;
                }
            }
        }
    }

    for (i = 0; i <= p_len-2; ++i) {
        bmGs[p_len-1-suff[i]] = p_len-1 -i;
    }
}

int bm_search(const char* a, const char* p)
{
    int a_len = strlen(a);
    int p_len = strlen(p);
    int* bmBc = new int[256];
    int* bmGs = new int[p_len];
    int ret = -1;

    preBc(p, p_len, bmBc);
    preGs(p, p_len, bmGs);

    int i = p_len-1;
    int j = p_len-1;
    for (; j >= 0; --i, --j) {
        while (p[j] != a[i]) {
            int c = bmBc[a[i]];
            int s = bmGs[i] + p_len-1-j;

            //i += p_len- j; //brute force
            //i += c; //only Bc
            //i += s; //only Gs
            i += std::max(c, s);
            if (i >= a_len) {
                i = -1;
                j = -1;
                break;
            }
            j = p_len - 1;
        }
    }

    ret = i+1;

    delete[] bmBc;
    delete[] bmGs;

    return ret;
}

int test_bm_search()
{
    const char* a = "abcbababcdefg";
    const char* p = "abab";

    int ret = bm_search(a, p);
    printf("str = %s\n", a);
    printf("search \"%s\" return %d\n", p, ret);
    return 0;
}

AUTO_RUN(test_bm_search)
