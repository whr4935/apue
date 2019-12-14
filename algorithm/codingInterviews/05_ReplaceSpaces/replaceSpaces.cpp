/*************************************************************************
 > File Name: replaceSpaces.cpp
 > Created Time: 2018年04月02日 星期一 08时30分39秒
 ************************************************************************/

#include <iostream>

void replaceSpaces(char* str)
{
    const char* alterStr = "%20";
    char* pAlter;
    int spaceCount = 0;
    char* p = str;
    while (*p) {
        if (*p == ' ')
            ++spaceCount;
        ++p;
    }
    char* originEnd = p-1;
    char* newEnd = p + 2*spaceCount-1;

    while (originEnd >= str) {
        if (*originEnd == ' ') {
            pAlter = (char*)alterStr+2;
            while (pAlter >= alterStr) {
                *newEnd-- = *pAlter--;
            }
        } else {
            *newEnd-- = *originEnd;
        }
        --originEnd;
    }
}

int main(int argc, char** argv)
{
    char str[200] = "a bc d  "; 
    replaceSpaces(str);
    std::cout << str << std::endl;

    return 0;
}

