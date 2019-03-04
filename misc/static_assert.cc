#include <type_traits>

class A
{

};

class B : A
{

};

template<typename T>
class D
{
    static_assert(std::is_base_of<A, T>::value, "T must derived from A");
};


int main(int argc, char *argv[])
{

    D<B> d;
    D<A> d2;
    
    return 0;
}
