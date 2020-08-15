#include <stdio.h>
#include <string.h>


int subsearch(const char* a, const char* p)
{
    int M = strlen(a);
    int N = strlen(p);

    for (int i = 0, j = 0; i <= M - N; ++i, j = 0) {
        while (a[i+j] == p[j]) {
            if (++j >= N) {
                return i;
            }
        }
    }

    return -1;
}



int main(int argc, char *argv[])
{
    const char* a = "abcabcagc";
    const char* p = "gcd";

    printf("result = %d\n", subsearch(a, p));

    return 0;
}
