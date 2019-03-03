/*************************************************************************
 > File Name: numericStrings.cpp
 > Created Time: 2018年04月11日 星期三 13时17分15秒
 ************************************************************************/

#include <iostream>

bool scanUnsignedInteger(const char** ppStr)
{
    const char* beg = *ppStr;

    if (*beg == '\0')
        return true;

    while (**ppStr>='0'&& **ppStr<='9') {
        ++(*ppStr);
    }

    return *ppStr > beg;
}

bool scanInteger(const char** ppStr)
{
    const char* beg = *ppStr;

    if (*beg == '\0')
        return true;

    if (*beg == '+' || *beg == '-')
        ++(*ppStr);

    return scanUnsignedInteger(ppStr);
}


bool isNumeric(const char* const numStr)
{
    bool ret = false;
    const char* str = numStr;

    ret = scanInteger(&str);
    if (!ret)
        return ret;

    if (*str=='.') {
        ++str;
        ret = scanUnsignedInteger(&str);
        if (!ret)
            return ret;
    }

    if (*str=='e' || *str=='E') {
        ++str;
        ret = scanInteger(&str);
        if (!ret)
            return ret;
    }

    return *str=='\0';
}

int main(int argc, char** argv)
{
    const char* numStr = "-1234.23e+4";

    std::cout << "numStr: " << numStr << std::endl;
    std::cout << std::boolalpha << "isNumeric: " << isNumeric(numStr) << std::endl;

    return 0;
}

