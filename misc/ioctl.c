#include <stdio.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
    printf("_IOC_WRITE:%d, _IOC_READ:%d", _IOC_WRITE, _IOC_READ);

    return 0;
}
