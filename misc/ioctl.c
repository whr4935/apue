#include <sys/ioctl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("%d\n", _IOC_WRITE);
    return 0;
}
