#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

double slow(double x)
{
        return pow(x,1.12345);
}

double slower(double x)
{
        return 1.0/x;
}

double slowest(double x)
{
        return sqrt(x);
}

void sig_handler(int sig)
{
    exit(0);
}


int main (int argc, char *argv[])
{

        signal(SIGINT, sig_handler);
        int i;
        double x;

        for (i = 0;i < 3000000; i++){
                x = 100.0;
                x = slow(x);
                x = slower(x);
                x = slowest(x);
        }

        while (1) {

        }

        return 0;
}
