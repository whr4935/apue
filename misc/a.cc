#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int gQuit = false;

void signalHandle(int sig)
{
    printf("signal:%d %s\n", sig, strsignal(sig));
    //gQuit = true;
}

#define N(x) (-x)


int main(int argc, char *argv[])
{
    int i;

    printf("isblank:%d\n", isblank(' '));

    printf("getpgrp:%d, getpid:%d\n", getpgrp(), getpid());
    printf("getsid:%d, getpid:%d\n", getsid(0), getpid());
    printf("tcgetpgrp:%d\n", tcgetpgrp(0));

    int a = -3;
    int b = -a;
    printf("b = %d\n", b);


    return 0;


    for (i = 1; i < 32; ++i) {
        if (i == SIGTSTP)
            continue;
        //signal(i, signalHandle);

    }

    while (!gQuit) {
        sleep(1);
    }

    return 0;
}
