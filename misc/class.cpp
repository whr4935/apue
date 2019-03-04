#include <iostream>
#include <algorithm>

struct A
{
    A(int d) 
    : ma{0}
    {
        std::cout << d << std::endl;
    }

    ~A() {
        std::cout << "~A" << std::endl;
    }

    int ma[4];
};


A testFunc(int d)
{
    return A(d);
}

int main(int argc, char *argv[])
{
    A aa1[] = {A(1)};
    

    A aa[] {testFunc(2), testFunc(3)};
    std::cout << "xxx" <<std::endl;
    printf("%llu\n", 123ull);


    //std::for_each(std::begin(a.ma), std::end(a.ma), [] (int d) {
            //std::cout << d << " ";
            //});
    
    return 0;
}
