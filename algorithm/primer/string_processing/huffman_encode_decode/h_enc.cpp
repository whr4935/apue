/*************************************************************************
 > File Name: h_enc.cpp
 > Created Time: 2017年11月02日 星期四 22时43分44秒
 ************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define COUNT 256

const char* msg;
int g_count[COUNT*2];
int g_heap[COUNT*2];
int g_dad[COUNT*2];
int g_code[COUNT];
int g_bitCount[COUNT];


static inline char bits(int num, int k, int j)
{
    return (num>>k) & ~(~0<<j);
}

// N -> 2N+1 2N+2
// N -> N/2-1
static void pqdowmheap(int heap[], int N, int k)
{
    int v, j;
    v = heap[k];
    while (k <= N/2-1) {
        j = k+k+1;
        if (j<N-1 && g_count[heap[j+1]] < g_count[heap[j]])
            ++j;
        if (g_count[v] <= g_count[heap[j]])
            break;
        heap[k] = heap[j];
        k = j;
    }
    heap[k] = v;
}

static void init_count(const unsigned char *msg, int length)
{
    memset(g_count, 0, sizeof(g_count));

    for (int i = 0; i < length; ++i) {
        ++g_count[(unsigned int)msg[i]];
    }
}

static void huffman_encode(const unsigned char *msg, int length, int** code, int** bits_count)
{
    int i;
    int N;
    int t;

    init_count(msg, length);

    N = 0;
    for (i = 0; i < COUNT; ++i) {
        if (g_count[i])
            g_heap[N++] = i;
    }     

    for (i = N/2-1; i >= 0; --i)
        pqdowmheap(g_heap, N, i);

    while (N > 1) {
        t = g_heap[0];
        g_heap[0] = g_heap[N-1];
        pqdowmheap(g_heap, --N, 0);
        /* printf("litter two:%d %d\n", g_count[t], g_count[g_heap[0]]); */
        g_count[COUNT+N-1] = g_count[t] + g_count[g_heap[0]];
        g_dad[t] = COUNT+N-1;
        g_dad[g_heap[0]] = -(COUNT+N-1);
        g_heap[0] = COUNT+N-1;
        pqdowmheap(g_heap, N, 0);
    } 
    g_count[COUNT] = 0;

    int j, k;
    int x;
    for (i = 0; i < COUNT; ++i) {
        if (!g_count[i])
            g_code[i] = 0;
        else {
            j = 0;
            k = 0;
            x = 1;
            t = g_dad[i];
            while (t) {
                if (t < 0) {
                    t = -t;
                    j += x;
                }
                ++k;
                x = x + x;
                t = g_dad[t];
            }
            g_bitCount[i] = k;
            assert((unsigned)k <= sizeof(int)*8);
            g_code[i] = j;
        }
    }
    
    *code = g_code;
    *bits_count = g_bitCount;

}

void print_result(int code[], int bits_count[])
{
    int i, j;

    int N = 0;
    int count;
    for (i = 0; i < COUNT; ++i) {
        if (g_count[i]) {
            g_heap[N++] = i;
        }
    }

    for (i = N/2-1; i>=0; --i)
        pqdowmheap(g_heap, N, i);

    count = N;
    while (N > 1) {
        j = g_heap[0];
        g_heap[0] = g_heap[N-1];
        g_heap[N-1] = j;
        pqdowmheap(g_heap, --N, 0);
    }
    N = count;

    int v;
    for (i = 0; i < N; ++i) {
        v = g_heap[i];
        if (bits_count[v]) {
            printf("%c(%02x):%4d", isprint(v) ? v : ' ', v, g_count[v]);
            printf("[ ");
            for (j = bits_count[v]-1; j >= 0; --j) {
                printf("%1d", bits(code[v], j, 1));
            }
            printf(" ]");
            printf("\n");
        }
    }
}

int save_tree_file(FILE *fout)
{
    int i;
    char buf[10];
    int len;
    int ret;
    int write_cnt = 0;
    int f_pos = ftell(fout);
    assert(f_pos ==0);
    fwrite(&write_cnt, 1, sizeof(write_cnt), fout);
    for (i = 0; i < COUNT; ++i) {
        len = 0;
        if (g_bitCount[i]) {
            buf[len++] = (char)i;
            buf[len++] = (char)g_bitCount[i];
            memcpy(buf+len, g_code+i, sizeof(*g_code));
            len += sizeof(*g_code);
            ret = fwrite(buf, 1, len, fout);
            if (ret != len) {
                printf("fwrite error:%s\n", strerror(errno));
                break;
            }
            write_cnt += len;
        }
    }
    fseek(fout, f_pos, SEEK_SET);
    fwrite(&write_cnt, 1, sizeof(write_cnt), fout);
    fseek(fout, 0, SEEK_END);

    return 0;
}


void write_bits(char* buf, int &pos, int d, int k)
{
    int p_bytes = pos/8;
    int left_bits = 8-pos%8;

    int n = left_bits <= k ? left_bits : k;
    while (n) {
        buf[p_bytes] |= (d>>(k-n))<<(left_bits-n);
        d &= ~(~(~0<<n)<<(k-n));
        k -= n;
        pos += n;
        p_bytes = pos/8;
        left_bits = 8 - pos%8;
        n = left_bits <= k ? left_bits : k;
    }
}

int encode_file(FILE* fout, const unsigned char* msg, int length)
{
    fwrite(&length, 1, sizeof(length), fout);

    int write_cnt = 0;

    int i = 0;
    int c;
    char buf[5] = {0};
    int pos = 0;
    unsigned int len;
    for (i = 0; i < length; ++i) {
        c = msg[i];
        write_bits(buf, pos, g_code[c], g_bitCount[c]);
        assert(pos <= 40);
        len = pos / 8;
        if (len) {
            if (fwrite(buf, 1, len, fout) != len) {
                printf("fwrite error:%s\n", strerror(errno));
                return -1;
            }
            write_cnt += len;

            buf[0] = buf[len];
            memset(buf+1, 0, sizeof(buf)/sizeof(*buf)-1);
            pos = pos % 8;
        }
    }

    if (pos > 0) {
        len = 1;
        if (fwrite(buf, 1, len, fout) != len) {
            printf("fwrite error:%s\n", strerror(errno));
            return -1;
        }
        write_cnt += len;
    }

    printf("write_cnt:%d\n", write_cnt);

    return write_cnt;
}


int main(int argc, char **argv)
{
    unsigned char *msg;

    if (argc != 2) {
        printf("Usage: %s filename\n", basename(argv[0]));
        exit(0);
    }

   FILE* fp = fopen(argv[1], "r"); 
   if (fp == NULL) {
       perror("fopen");
       exit(1);
   }

   fseek(fp, 0, SEEK_END);
   int file_length = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   printf("file length:%d byte(s)\n", file_length);
   msg = (unsigned char*)malloc(file_length);
   if (msg == NULL) {
       printf("allocate msg buffer failed\n");
       exit(1);
   }
   size_t read_size = fread(msg, 1, file_length, fp);
   if (read_size == 0) {
       printf("fread failed:%s\n", strerror(errno));
       exit(1);
   }
    // printf("read_size:%lu\n", read_size);

    int* code;
    int* bits_count;
    huffman_encode(msg, file_length, &code, &bits_count);
    // print_result(code, bits_count);

    char enc_file_name[100];
    strcpy(enc_file_name, basename(argv[1])); 
    strcat(enc_file_name, ".huf");

    FILE* fout = fopen(enc_file_name, "w");
    if (fout == NULL) {
        printf("fopen %s failed:%s\n", enc_file_name, strerror(errno));
        return -1;
    }

    save_tree_file(fout);
    int write_cnt = encode_file(fout, msg, file_length);
    fclose(fout);

    printf("compress rate:%.1f%%\n", write_cnt*100.0/file_length);

    free(msg);
    fclose(fp);

    return 0;
}
