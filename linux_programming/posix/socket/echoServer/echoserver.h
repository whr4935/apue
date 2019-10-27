#ifndef ECHOSERVER_H
#define ECHOSERVER_H


#include <string>
#include <netinet/in.h>

class EchoServer
{
public:
    EchoServer();
    virtual ~EchoServer();

    virtual std::string name();
    virtual int init();
    virtual int destroy();
    virtual int run();
    virtual void str_echo(int connfd);

protected:
    int mListenFd;
    struct sockaddr_in mServAddr;
    std::string mName = "EchoServer";
};

#endif // ECHOSERVER_H
