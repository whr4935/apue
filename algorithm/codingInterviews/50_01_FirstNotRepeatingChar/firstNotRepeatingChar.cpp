/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>

char firstNotRepeatingChar(const std::string &str)
{
    int *hashTable = new int[256];

    for (unsigned int i = 0; i < str.size(); ++i) {
        ++hashTable[(int)str[i]];
    }

    for (unsigned int i = 0; i < str.size(); ++i) {
        if (hashTable[(int)str[i]] == 1)
            return str[i];
    }

    return 0;
}

int main(int argc, char *argv[])
{
    std::cout << firstNotRepeatingChar(std::string("abaccdeff")) << std::endl;
    return 0;
}
