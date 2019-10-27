#include <utils/utils.h>
#include <utils/network.h>
#include <utils/factory.h>
#include "echoserver.h"


EchoServer::EchoServer()
{
}

EchoServer::~EchoServer()
{

}

std::string EchoServer::name()
{
    return mName;
}

int EchoServer::init()
{
    mListenFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&mServAddr, sizeof(mServAddr));
    mServAddr.sin_family = AF_INET;
    mServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mServAddr.sin_port = htons(SERV_PORT);

    bind(mListenFd, (SA*)&mServAddr, sizeof(mServAddr));
    listen(mListenFd, LISTENQ);

    return 0;
}

int EchoServer::destroy()
{
    close(mListenFd);

    return 0;
}

int EchoServer::run()
{
    int connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    pid_t childpid;

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(mListenFd, (SA*)&cliaddr, &clilen);

        std::cout << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << std::endl;

        if ((childpid = fork()) == 0) {
            close(mListenFd);
            str_echo(connfd);
            std::cout << "exit" << std::endl;
            exit(0);
        }
        close(connfd);
    }
}

void EchoServer::str_echo(int connfd)
{
    ssize_t n;
    char buf[MAXLINE];

    for (;;) {
        if ((n = read(connfd, buf, MAXLINE)) > 0) {
            std::cout << buf;
            write(connfd, buf, n);
        } else {
            if (errno != EINTR)
                break;
        }
    }
}

REGISTER_CLASS_AUTO(EchoServer)


