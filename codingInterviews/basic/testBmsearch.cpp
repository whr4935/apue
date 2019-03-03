/*************************************************************************
 > File Name: testBmsearch.cpp
 > Created Time: 2018年04月09日 星期一 09时52分03秒
 ************************************************************************/

#include <iostream>
#include <Utilities/bmSearch.h>


int main(int argc, char** argv)
{
    const char* str = "abcddfdsfewfewrttg"; 
    const char* pattern = "ddfd";

    std::cout << "str: " << str << std::endl;
    std::cout << "search " << pattern << ": " << bmSearch(str, pattern) << std::endl;

    return 0;
}

