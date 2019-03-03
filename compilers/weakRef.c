
#include <pthread.h>
int printf(const char*fmt,...);
int pthread_craete(pthread_t*, const pthread_attr_t*,
        void*(*)(void*), void*) __attribute__((weak));

void* routine(void* arg)
{
    return (void*)NULL;
}

int main(int argc, char *argv[])
{
    printf("hello world!\n");

    /*pthread_t tid;*/
    /*pthread_create(&tid,NULL, routine, NULL);*/

    if (pthread_create) {
        printf("This is multi-thread version!\n");
    } else {
        printf("tis is single thread version!\n");
    }

    return 0;
}
