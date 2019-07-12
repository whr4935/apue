#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>

void benchmark_begin(BenchMark *bench)
{
    struct timeval val;

    gettimeofday(&val, NULL);

    bench->begin_time =  val.tv_sec * 1000 + (val.tv_usec + 500) / 1000;
}

void benchmark_end(BenchMark *bench, const char *fmt, ...)
{
    struct timeval val;

    gettimeofday(&val, NULL);
    bench->end_time = val.tv_sec * 1000 + (val.tv_usec + 500) / 1000;

    bench->diff = bench->end_time - bench->begin_time;

    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf(" cost: %.3fs\n", bench->diff / 1000.0);
}

void print_array(int a[], int N)
{
    static const int col_num = 8;
    int col = 0;

    while (N--) {
        printf("%4d", a[col++]);

        if ((col % col_num) == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

void print_index_array(int a[], int p[], int N)
{
    static const int col_num = 8;
    int col = 0;

    while (N--) {
        printf("%-4d", a[p[col++]]);

        if ((col % col_num) == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

