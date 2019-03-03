/*************************************************************************
 > File Name: h_dec.cpp
 > Created Time: 2017年11月04日 星期六 23时37分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

struct node {
    int b;
    struct node *l, *r;
} g_head = {
    -1,
};


static inline int bits(int d, int j, int k)
{
    return (d>>j)&~(~0<<k); 
}


int construct_huffman_tree(FILE *fin)
{
    int tree_size;
    unsigned char buf[6];
    int d;
    int bits_count;
    int code;
    int b;
    struct node *x, **ppn;
    int len;

    fread(&tree_size, 1, sizeof(tree_size), fin);
    assert(tree_size%6==0);

    while (tree_size) {
        len = fread(buf, 1, 6, fin);
        if (len != 6) {
            // printf("fread return %d\n", len);
            break;
        }
        tree_size -= len;

        d = buf[0];
        bits_count = buf[1];
        code = *(int*)(buf+2);

        x = &g_head;
        for (int i = bits_count-1; i>=0; --i) {
            b = bits(code, i, 1);
            ppn = b ? &x->r : &x->l;
            if (!*ppn) {
                *ppn = (struct node*)malloc(sizeof(struct node));
                (*ppn)->l = NULL;
                (*ppn)->r = NULL;
                (*ppn)->b = -1;
            }
            x = *ppn;
        }
        x->b = d;
    }

    return 0;
}

int restore_byte(char* buf, int &pos, int max_pos)
{
    int b;
    int p_bytes;
    int p_bits;
    struct node *x;
    int pos_bak = pos;

    x = &g_head;
    for (;;) {
        if (pos == max_pos) {
            pos =pos_bak;
            return -1;
        }

        p_bytes = pos/8;
        p_bits = 7-pos%8;
        b = bits(buf[p_bytes], p_bits, 1);
        x = b ? x->r : x->l;
        ++pos;
        if (x->r == NULL && x->l == NULL)
            break;

    }

    return x->b;
}

int decode_huffman_file(FILE* fin, FILE* fout)
{
    int file_length;
    int byte_count=0;
    char buf[10];
    const int total_byte = 10;
    int left_byte;
    int left_bits;
    int len; 
    fread(&file_length, 1, sizeof(int), fin);
    printf("file_length:%d\n", file_length);

    int byte;
    int max_pos = 0;
    int pos = 0;
    left_byte = 0;
    for (;;) {
        len = fread(buf+left_byte, 1, total_byte-left_byte, fin);
        if (len == 0) {
            break;
        }

        max_pos =80;
        for (;;) {
            byte = restore_byte(buf, pos, max_pos);
            if (byte >= 0) {
                // printf("restore_byte:%02x\n", byte);
                fputc(byte, fout);
                byte_count++;
                if (byte_count == file_length) {
                    printf("restore finished!\n");
                    goto exit;
                }
                continue;
            }
            assert(byte==-1);

            left_bits = max_pos - pos;
            if (left_bits == 0) {
                left_byte = 0;
                pos = 0;
            } else {
                left_byte = left_bits/8+1;
                pos = 8 - left_bits % 8;
                for (int i = 0; i < left_byte; ++i) {
                    buf[i] = buf[total_byte-left_byte+i];
                }
            }
            break;
        }
    }


exit:
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage:%s enc_file\n", basename(argv[0]));
        return 0;
    }

    char* ext = strrchr(argv[1], '.');
    if (ext==NULL || strcmp(ext+1, "huf")) {
        printf("invalid arguments!\n");
        return -1;
    }

    FILE* fin = fopen(argv[1], "r");
    if (fin == NULL) {
        printf("fopen %s failed:%s\n", argv[1], strerror(errno));
        return -1;
    }

    char dec_file_name[100];
    strcpy(dec_file_name, basename(argv[1]));
    strcpy(dec_file_name+strlen(dec_file_name)-3, "dec");
    

    FILE* fout = fopen(dec_file_name, "w");
    if (fout == NULL) {
        printf("fopen %s failed:%s\n", dec_file_name, strerror(errno));
        return -1;
    }

    construct_huffman_tree(fin);
    decode_huffman_file(fin, fout);


    fclose(fout);
    fclose(fin);

    return 0;
}
