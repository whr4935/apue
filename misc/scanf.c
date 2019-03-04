
#include <stdio.h>
#include <string.h>

void fun1(char* p)
{

}

int main(int argc, char *argv[])
{
    const char *testStr = "123 456";

    char buf[2];
    memset(buf, -1, 10);
    

    int ret = 0;
    int tmp;

    ret = sscanf(testStr, "%2s", buf);
    printf("ret = %d, buf:%s\n", ret, buf);

    return 0;


    ret = sscanf(testStr, "%d %d", &tmp, &tmp);

    printf("ret = %d, tmp:%d\n", ret, tmp);


    char* p = NULL;
    char buf2[] = "123\n";
    char* pp = buf2;

    /*p = strtok(buf2, " \n");*/
    /*while (p) {*/
        /*printf("p = %s\n", p);*/
        /*p = strtok(NULL, " ");*/
    /*}*/


    while ((p = strsep((char**)&pp, " \n")) && *p) {
        printf("p = %s\n", p);
    }


    char* p1, *p2;
    p1 = p;

    p2 = p1;

    return 0;
}
