#include <string.h>
#include <stdio.h>

extern int myadd(int a, int b, int* c);
extern int mywrite(int, char*, int len);

int main(int argc, char *argv[])
{
    char buf[1024];
    int a, b, res;
    char* mystr = "Calculating...\n";
    char* emsg = "Error in adding\n";

    a = 5; b = 10;
    mywrite(1, mystr, strlen(mystr));
    if (myadd(a, b, &res)) {
        sprintf(buf, "The result is %d\n", res);
        mywrite(1, buf, strlen(buf));
    } else {
        mywrite(1, emsg, strlen(emsg));
    }

    return 0;
}
