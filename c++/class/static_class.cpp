#include "static_class.h"
#include <iostream>

int A::sData = 3;

int test_static_class_member()
{
    std::cout << &A::sData << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    
    return 0;
}
