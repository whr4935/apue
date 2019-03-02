#include <iostream>
#include <functional>

template<typename T>
struct A
{
    static T test(T t) {
        std::cout << t << std::endl;

        return t;
    }
};


template<typename T>
struct A<T&&>
{
    static T test(T t) {
        std::cout << "v2: " << t << std::endl;

        return t;
    }

};

enum {
    kWhatB,
};

template<int K=2>
struct  B
{
    static const int d = K;
};

template<>
struct B<kWhatB>
{
    static const int d = 10;
};


int main(int argc, char *argv[])
{
    A<float&&>::test(12);
    B<0> b;
    std::cout << b.d << std::endl;

    return 0;
}
