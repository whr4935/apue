#include <stdio.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    void* addr = mmap(NULL, 120, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
    if (addr == MAP_FAILED) {
        printf("map failed!\n");
    }
    printf("addr:%p\n", addr);

    *(int*)addr = 0x1234;

    printf("%#x\n", *(char*)addr);


    munmap(addr, 120);
    return 0;
}
