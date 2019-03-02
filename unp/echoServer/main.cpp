#include <unp.h>
#include <iostream>
#include <factory.h>
#include "echoserver.h"

using namespace std;

#define TEST_SERVER_NAME "EchoServer"

int main(int argc, char *argv[])
{
    EchoServer* s =  Factory<EchoServer>::instance()->getClass(TEST_SERVER_NAME);

    s->init();
    s->run();
    s->destroy();

    return 0;
}
