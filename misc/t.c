#include <stdio.h>
#include <time.h>

struct queue
{
    size_t in;
};


int main(int argc, char *argv[])
{
    struct queue q;
   q.in = 4;
    size_t a = q.in++;
    int b = a & 7;

    a = q.in++ & 8 -1;
    /*printf("a  = %d, b = %d, in:%d\n", a, b, q.in);*/

    printf("a:%f\n", 1e9);

    return 0;
}
