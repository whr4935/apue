#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define container_of(p, stru, node) ((stru*)((char*)p - (char*)(&(((stru*)0)->node))))

#define BEGIN_MODULE    printf("========= begin %s ==========\n", __FUNCTION__)
#define END_MODULE    printf("========== end %s ==========\n", __FUNCTION__)
#define TRACE() std::cout<< "Line:" << __LINE__ << " ==> " << __PRETTY_FUNCTION__ << std::endl

typedef struct {
    unsigned long long begin_time;
    unsigned long long end_time;
    unsigned long long diff;
}BenchMark;


void benchmark_begin(BenchMark *bench);
void benchmark_end(BenchMark *bench, const char *fmt, ...);
void print_array(int a[], int N);
void print_index_array(int a[], int p[], int N);


#endif
