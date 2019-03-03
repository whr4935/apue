/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

void permutation(char* str, char* begin)
{
    if (*begin == '\0')
        std::cout << str << "\n";
    else {
        for (char* pCh = begin; *pCh!='\0'; ++pCh) {
            char temp = *pCh;
            *pCh = *begin;
            *begin = temp;

            permutation(str, begin+1);

            temp = *pCh;
            *pCh = *begin;
            *begin = temp;
        }
    }
}

void permutation(char* str)
{
    if (str == nullptr)
        return;

    permutation(str, str);
}

int main(int argc, char *argv[])
{
    char str[] = "abc";    

    permutation(str);
    
    return 0;
}
