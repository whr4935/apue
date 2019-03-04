#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAKE_TAG(a, b, c, d) ((d) | (c)<<8 | (b)<<16 | (a)<<24)

int main(int argc, char *argv[])
{
    int x = MAKE_TAG('A', 'B', 'C', 'D');
    printf("%#X\n", x);

    const char* ip = "1.2.3.4";
    in_addr_t addr = inet_addr(ip);
    addr = ntohl(addr);
    printf("addr:%#x\n", addr);

    int a1 = 123llu;

    return 0;
}
