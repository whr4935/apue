#include <stdio.h>
#include <string.h>
#include "huffman_code.h"

#define COUNT 27

static int g_count[100];
static int g_dad[100];

static int index(char a)
{
    if (a >= 'a' && a <= 'z')
        return a - 'a' + 1;
    else
        return 0;
}
//heap数据下标从0开始，那么父节点下标 = (子节点下标-1)/2
// 0 --  1 2
// 1 --  3 4
// 2 --  5 6
static void pqdownheap(int heap[], int N, int k)
{
    int j;
    int v = heap[k];

    while (k <= N/2-1) {
        j = k + k + 1;
        if (j < N-1 && g_count[heap[j]] > g_count[heap[j+1]]) ++j;
        if (g_count[v] < g_count[heap[j]]) break;
        heap[k] = heap[j];
        k = j;
    }
    heap[k] = v;
}

void huffman_code(char *str, int M, char *code, int *bits_count)
{
    int i;
    int heap[100];
    int N = 0;

    for (i = 0; i < COUNT; ++i)
        g_count[i] = 0;

    for (i = 0; i < M; ++i)
        ++g_count[index(str[i])];

    for (i = 0; i < M; ++i) {
        if (g_count[i])
            heap[N++] = i;
    }

    for (i = N/2-1; i >= 0; --i)
        pqdownheap(heap, N, i);

    printf("test heap:\n");
    for (i = 0; i < N; ++i) {
        printf("%c: %d\n", heap[i]==0? ' ' : heap[i]-1+'a', g_count[heap[i]]);
    }

    int t;
    while (N > 1) {
        t = heap[0];
        heap[0] = heap[--N];
        pqdownheap(heap, N, 0);

        g_count[COUNT+N-1] = g_count[t] + g_count[heap[0]];
        g_dad[t] = COUNT+N-1;
        g_dad[heap[0]] = -COUNT-N+1;
        heap[0] = COUNT+N-1;
        pqdownheap(heap, N, 0);
    }
    g_dad[COUNT+N-1] = 0;

    int j;
    int x;
    int cnt;
    for (i = 0; i < COUNT; ++i) {
        if (g_count[i] == 0) {
            code[i] = 0;
            bits_count[i] = 0;
        } else {
            t = g_dad[i];
            x = 0;
            cnt = 0;
            j = 1;
            while (t) {
                if (t < 0) {
                    x += j;
                    t = -t;
                }
                j += j;
                ++cnt;
                t = g_dad[t];
            }
            code[i] = x;
            bits_count[i] = cnt;
        }
    }
}

static int inline bits(int n, int k, int j)
{
    return n>>k & ~(~0<<j);
}

//////////////////////////////////////
void test_huffman_code()
{
    char* str = "a simple string to be encoded";
    char code[COUNT] = {0};
    int bits_count[COUNT] = {0};

    huffman_code(str, strlen(str), code, bits_count);

    int i;
    int j;
    for (i = 0; i < COUNT; ++i) {
        printf("%c:%d", i==0?' ':i-1+'a', bits_count[i]);

        if (bits_count[i]) {
            printf("[ ");
            for (j = bits_count[i]-1; j >= 0; --j) {
                printf("%1d", bits(code[i], j, 1));
            }
            printf(" ]");
        }
        printf("\n");
    }


}
