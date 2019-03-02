#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd1, fd2;
    fd1 = open("a.txt", O_RDONLY);
    fd2 = open("a.txt", O_RDONLY);

    char c1, c2;
    c1 = c2 = 0;

    read(fd1, &c1, 1);
    read(fd2, &c2, 1);


    printf("c1:%c, c2:%c\n", c1, c2);


    return 0;
}
