#include <utils/utils.h>
#include <utils/network.h>
#include <utils/factory.h>
#include "echoClient.h"

EchoClient::EchoClient(const char* ip)
    :mServIp(ip)
{

}

EchoClient::~EchoClient()
{

}

int EchoClient::init()
{
    mCliFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&mServAddr, sizeof(mServAddr));
    mServAddr.sin_family = AF_INET;
    mServAddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, mServIp.c_str(), &mServAddr.sin_addr.s_addr) < 0) {
        err_quit("inet_pton failed!");
    }

    socklen_t servlen;
    servlen = sizeof(mServAddr);
    connect(mCliFd, (SA*)&mServAddr, servlen);

    std::cout << "connect success!" << std::endl;

    return 0;
}

int EchoClient::destroy()
{
    close(mCliFd);

    return 0;
}

int EchoClient::run()
{
    str_cli(stdin, mCliFd);

    return 0;
}

void EchoClient::str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (write(sockfd, sendline, strlen(sendline)) <= 0) {
            if (errno != EINTR)
                break;
        }
        read(sockfd, recvline, MAXLINE);
        std::cout << recvline;
    }
}

REGISTER_CLASS_AUTO(EchoClient)
