#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    const char* src = "abcde";
   char buf[4]; 
   snprintf(buf, 4, "%s", src);


    return 0;
}
