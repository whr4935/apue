#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <string>
#include <netinet/in.h>

class EchoClient
{
public:
    EchoClient(const char* ip = "127.0.0.1");
    virtual ~EchoClient();

    virtual int init();
    virtual int destroy();
    virtual int run();
    virtual void str_cli(FILE *fp, int sockfd);

protected:
    std::string mServIp;
    int mCliFd;
    struct sockaddr_in mServAddr;
};

#endif // ECHOCLIENT_H
