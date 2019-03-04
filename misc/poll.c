#include <poll.h>

int main(int argc, char *argv[])
{

    struct pollfd s;
    s.events = POLLIN;
    return 0;
}
