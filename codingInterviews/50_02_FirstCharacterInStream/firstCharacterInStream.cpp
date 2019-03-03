/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <string>
#include <cstring>

void firstCharacterInStream(const std::string &str)
{
    unsigned int i = 0;
    int hashTable[256];
    memset(hashTable, -1, 256*sizeof(int));

    for (i = 0; i < str.size(); ++i) {
        if (hashTable[(int)str[i]] < 0) {
            std::cout << i << ": " << str[i] << std::endl;
        }
        hashTable[(int)str[i]] = i;
    }
}

int main(int argc, char *argv[])
{
    firstCharacterInStream("google");
    return 0;
}
