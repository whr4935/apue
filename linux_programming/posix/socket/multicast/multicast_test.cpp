#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <utils/utils_main/utils_main.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

int test_join_multicast()
{
    printf("hello\n");

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        printf("create udp socket failed!\n");
        return -1;
    }

    const char* addr = "239.1.1.1";
    const int multicast_port = 8208;

    int reuse_socket = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(reuse_socket));
    if (ret < 0) {
        printf("resue socket fialed! %s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in sockAddr{};
    sockAddr.sin_family = AF_INET;
    inet_aton(addr, &sockAddr.sin_addr);
    //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(multicast_port);
    //sockAddr.sin_port = 0;
    ret = bind(fd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
    if (ret < 0) {
        printf("bind sockAddr failed!\n");
        return -1;
    }


    struct sockaddr_in localAddr;
    socklen_t localAddrLen = sizeof(localAddr);
    ret = getsockname(fd, (struct sockaddr*)&localAddr, &localAddrLen);
    if (ret < 0) {
        printf("getsockname failed!\n");
        return -1;
    }

    int localPort = ntohs(localAddr.sin_port);
    printf("get port = %d\n", localPort);

    struct ip_mreq_source multi_source{};
    inet_aton(addr, &multi_source.imr_multiaddr);
    //inet_aton("192.168.1.104", &multi_source.imr_sourceaddr);
    multi_source.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(fd,IPPROTO_IP, IP_ADD_MEMBERSHIP, &multi_source, sizeof(multi_source));
    if (ret < 0) {
        printf("join multicast failed! %s\n", strerror(errno));
        return -1;
    }
    printf("join multicast success!\n");

    uint8_t buf[4096];
    socklen_t sockLen = sizeof(sockAddr);
    //int flags;
    //flags = fcntl(fd, F_GETFL);
    //flags |= O_NONBLOCK;
    //fcntl(fd, F_SETFL, flags);

    for (;;) {
        ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&sockAddr, &sockLen);
        if (ret < 0) {
            printf("receive multicat failed, %s %d\n", strerror(errno), errno);
            return -1;
        }

        int len = ret < 16 ? ret : 16;
        for (size_t i = 0; i < len; ++i) {
            printf("data:%02x ", buf[i]);
        }
        printf("\n");
        break;
    }

    ret = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multi_source, sizeof(multi_source));
    if (ret < 0) {
        printf("leave multicast failed, %s\n", strerror(errno));
        return -1;
    }

    printf("leave multicast success!\n");

    close(fd);
    return 0;
}

AUTO_RUN(test_join_multicast)
