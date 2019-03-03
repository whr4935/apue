/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>
#include <string.h>

//from bottom to top
int translateNumbers(int number)
{
    std::string str = std::to_string(number);
    int len = str.size();
    int *counts = new int[len];
    memset(counts, 0, len);
    int count = 0;

    for (int i = len-1; i >= 0; --i) {
        if (i < len-1)
            count = counts[i+1];
        else
            count = 1;

        if (i < len-1) {
            int digit1 = str[i] - '0';
            int digit2 = str[i+1] - '0';
            int d = digit1*10 + digit2;
            if (d >= 10 && d <= 25) {
                if (i < len-2) {
                    count += counts[i+2];
                } else {
                    ++count;
                }
            }
        }
        counts[i] = count;
    }

    delete [] counts;
    return count;
}

int main(int argc, char *argv[])
{
    std::cout << translateNumbers(12258) << std::endl;

    return 0;
}
