#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

size_t av_strlcpy(char *dst, const char *src, size_t size)
{
        size_t len = 0;
            while (++len < size && *src)
                        *dst++ = *src++;
                if (len <= size)
                            *dst = 0;
                    return len + strlen(src) - 1;

}

size_t av_strlcat(char *dst, const char *src, size_t size)
{
        size_t len = strlen(dst);
            if (size <= len + 1)
                        return len + strlen(src);
                return len + av_strlcpy(dst + len, src, size - len);

}

void test_convert()
{
    char str[] = "igmp://239.11.0.10:5140?mediaType=1&channelFCCPort=8027&channelFCCIP=183.223.122.117&channelFECPort=5139" ;
    char*p = str;
    while (p = strstr(p, "channel"))
    {
        *p -= 32;
        p += 7;
    }
    puts(str);
}

void test_ip_conver()
{
    char srcIp[] = "223.85.247.182";
    in_addr_t addr = inet_addr(srcIp);
    printf("net addr:%#x, ntohl(addr):%#x\n", addr, ntohl(addr));
}


int main(int argc, char *argv[])
{
    /*test_convert();*/
    /*return 0;*/


    test_ip_conver();

    return 0;
    char play_seektime[100];
    memset(play_seektime, 'A', 100);
    const char* uri = "fdsfsfdsfdsfds&playseek=20181217T131351Zabc";
       const char* playseek = strstr(uri, "playseek=");
       if (playseek) {
           const char*p1 = playseek+9;
           const char* p2 = NULL;
           if (*p1) {
               printf("[%s:%d] layseek=%s\n", __FUNCTION__, __LINE__, p1);
               p2 = strrchr(p1, 'Z');
               if (p2 && p2-p1>9 && p1[8]=='T') {
                   size_t len = 0;
                   av_strlcpy(play_seektime, p1, 17);
                   /*strncpy(play_seektime, p1, 16);*/
                   printf("len = %zu\n", len);
                   /*printf("playseek:%p\n", play_seektime);*/
                   /*printf("playseek + len:%p\n", play_seektime+len);*/
                   av_strlcpy(play_seektime, p1, p2 - p1 + 2);
                   /*strncat(play_seektime, "-\r\n", 3);*/
                   printf("[%s:%d] cmd: %s\n", __FUNCTION__, __LINE__, play_seektime);
               }
           }
       } 

    return 0;
}
