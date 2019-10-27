#include <utils/utils.h>
#include <utils/utils_main/utils_main.h>
#include <iostream>
#include <utils/factory.h>
#include "echoserver.h"

using namespace std;

#define TEST_SERVER_NAME "EchoServer"

int TEST_echoserver(int argc, char *argv[])
{
    EchoServer* s =  Factory<EchoServer>::instance()->getClass(TEST_SERVER_NAME);

    s->init();
    s->run();
    s->destroy();

    return 0;
}

AUTO_RUN2(TEST_echoserver)
