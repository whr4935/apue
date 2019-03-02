#include <iostream>
#include <unp.h>
#include <stdio.h>
#include <factory.h>
#include <EchoClient.h>

using namespace std;

int main(int argc, char *argv[])
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
