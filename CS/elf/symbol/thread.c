#include <pthread.h>
int pthread_create (pthread_t *__restrict __newthread,
               const pthread_attr_t *__restrict __attr,
               void *(*__start_routine) (void *),
               void *__restrict __arg) __THROWNL __nonnull ((1, 3)) __attribute__ ((weak));

int printf(const char* format, ...);

int main(int argc, char *argv[])
{
    if (pthread_create) {
        printf("This is multi-thread version!\n");
    } else {
        printf("single, pthread_create:%p\n", pthread_create);
    }
    return 0;
}
