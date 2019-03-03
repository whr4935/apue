#include "algorithm.h"
#include <sys/time.h>

int initUtil()
{
    srand(time(NULL));

    return SUCCESS;
}

int uninitUtil()
{
    return SUCCESS;
}

int printArry(element_type *e, size_t size)
{
    size_t i;

#if DEBUG_PRINT
    printf("%d ", e[0]);

    for (i = 1; i < size; ++i) {
        if (!(i % 10))
            printf("\n");

        printf("%d ", e[i]);
    }
    printf("\n");
#endif

    return 0;
}

int statisticTimeFunc(int(*pf)(void *), void *arg, char *fmt, ...)
{
    int ret;
    struct timespec start, end, diff;
    va_list ap;
    char buffer[256];

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (pf)
        ret = (*pf)(arg);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (end.tv_nsec < start.tv_nsec) {
        end.tv_nsec += 1e9;
        end.tv_sec -= 1;
    }
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    printf("[%s] time: %.3fs\n", buffer, diff.tv_sec + diff.tv_nsec/1e9);

    return ret;
}
