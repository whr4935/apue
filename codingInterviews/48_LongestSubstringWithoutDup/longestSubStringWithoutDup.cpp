/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>
#include <cstring>

int longestSubStringWithoutDup(const std::string &str)
{
    int len = 0;
    int i = 0;
    int j = 0;
    int d = 0;

    int *l = new int[str.size()];
    memset(l, 0, str.size());

    for (i = 1; i < (int)str.size(); ++i) {
        for (d = -1, j = 0; j < i; ++j) {
            if (str[j] == str[i])
                d = j;
        }
        if (d < 0 || i-d > l[i-1]) {
            l[i] = l[i-1] + 1;
        } else {
            l[i] = i-d;
        }

        if (l[i] > len) {
            len = l[i];
        }
    }

    delete [] l;

    return len;
}

int main(int argc, char *argv[])
{
    std::cout << longestSubStringWithoutDup("arabcacfr") << std::endl;

    return 0;
}
