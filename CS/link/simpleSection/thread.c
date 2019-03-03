#include <pthread.h>
int pthread_create(pthread_t*, const pthread_attr_t*, void*(*)(void*), void*) __attribute__((weak));

int printf(const char* format, ...);

int main(int argc, char *argv[])
{
    if (pthread_create) {
        printf("This is multi-thread version!\n");
    } else {
        printf("single\n");
    }
    return 0;
}
