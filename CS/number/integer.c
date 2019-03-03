#include <stdio.h>
#include <string.h>

float sum_elements(float a[], size_t len)
{
    int i;
    float result = 0;

    for (i = 0; i <= len-1; ++i)
        result += a[i];
    return result;
}

int strlonger(const char* a, const char *b)
{
    return strlen(a) > strlen(b) ;
}

int main(int argc, char *argv[])
{
    char n = -128; 
    /*char n2 = -47;*/
    unsigned short n2 = n;
    /*n2 *= n;*/
    printf("%d\n", (short)n2);

    int x = 53191;
    short ax = (short)x;

    /*sum_elements(NULL, 0);*/

    int result =  strlonger("ab", "abc");
    result = strlonger("ab", "ab");
    
    return 0;
}
