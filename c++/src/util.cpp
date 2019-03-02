#include <c_plus_plus_primer.h>
/* #include <windows.h> */
#include <cstdarg>


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
    //LARGE_INTEGER beg, end, freq;
    long long diff, diff_s;
    long double diff_time, diff_ms;
    int ret;
    va_list ap;
    char buffer[256];

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    //QueryPerformanceFrequency(&freq);

    //QueryPerformanceCounter(&beg);
    if (pf)
        ret = (*pf)(arg);
    //QueryPerformanceCounter(&end);

    //diff = end.QuadPart - beg.QuadPart;
    //diff_time = (long double)diff / freq.QuadPart;

#if 0
    diff_s = (long long)diff_time;
    diff_ms = (diff_time - diff_s) * 1000;

    printf("[%s] time: %llds %.3Lfms\n", buffer, diff_s, diff_ms);
#else
    printf("[ %s ] time: %.3Lfs\n", buffer, diff_time);

#endif
    return ret;
}
