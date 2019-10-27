#include <utils/utils.h>
#include <utils/network.h>
#include <time.h>
#include <utils/utils_main/utils_main.h>

int TEST_timeserver(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    for (;;) {
        connfd = accept(listenfd, (SA*)NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }

    return 0;
}

AUTO_RUN2(TEST_timeserver)
