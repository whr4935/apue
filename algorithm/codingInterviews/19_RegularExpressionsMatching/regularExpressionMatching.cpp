/*************************************************************************
 > File Name: regularExpressionMatching.cpp
 > Created Time: 2018年04月11日 星期三 10时01分45秒
 ************************************************************************/

#include <iostream>

bool matchCore(char* str, char* pattern);

bool match(char* str, char* pattern)
{
    if (str == nullptr || pattern == nullptr)
        return false;

    return matchCore(str, pattern);
}

bool matchCore(char* str, char* pattern)
{
    if (*str=='\0')
        return true;

    if (*str!='\0' && *pattern=='\0')
        return false;

    if (*(pattern+1)=='*') {
        if (*pattern == *str || *pattern=='.') {
            return matchCore(str, pattern+2) ||
                matchCore(str+1, pattern+2) ||
                matchCore(str+1, pattern);
        } else {
            return matchCore(str, pattern+2);
        }
    }

    if (*pattern==*str || *pattern=='.') {
        return matchCore(str+1, pattern+1);
    }

    return false;
}

int main(int argc, char** argv)
{
    char str[] = "abcd";
    char pattern[] = "abc";

    std::cout << "str: " << str << std::endl;
    std::cout << "pattern: " << pattern << std::endl;
    std::cout << std::boolalpha << match(str, pattern) << std::endl;

    return 0;
}

