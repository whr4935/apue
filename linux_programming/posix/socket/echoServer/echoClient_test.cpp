#include <iostream>
#include <stdio.h>
#include <utils/factory.h>
#include <utils/utils_main/utils_main.h>
#include "echoClient.h"

using namespace std;

int TEST_echoClient(int argc, char *argv[])
{
    EchoClient* client = Factory<EchoClient>::instance()->getClass("EchoClient");

    if (client == nullptr) {
        std::cout << "get client null" << std::endl;
        exit(0);
    }

    client->init();
    client->run();
    client->destroy();

    return 0;
}

AUTO_RUN2(TEST_echoClient)
