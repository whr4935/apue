#include <iostream>

template<typename T>
struct remove_reference 
{
    typedef T type;
};

//partial implementation
template<typename T>
struct remove_reference<T&>
{
    typedef T type;
};

template<typename T>
struct remove_reference<T&&>
{
    typedef T type;
};

//move
template<typename T>
inline typename remove_reference<T>::type&& move(T&& __t)
{
    return static_cast<typename  remove_reference<T>::type&&>(__t);
}

//forward
template<typename T>
inline T&& forward(typename remove_reference<T>::type& __t)
{
    return static_cast<T&&>(__t);
}

template<typename T>
inline T&& forward(typename remove_reference<T>::type&& __t)
{
    return static_cast<T&&>(__t);
}

/////////////////////////////////////////////////////////////////
struct X {};

void inner(const X&) {std::cout << "inner(const X&)" << std::endl;}
void inner(X&&) {std::cout << "inner(X&&)" << std::endl;}

template<typename T>
void outer(T&& t) {
    inner(forward<T>(t));
}


int test_move()
{
    int a = 10;
    int& a1 = a;
    int&& a2 = move(a);
    int&& a3 = move(a1);
    int&& a4 = move(a2);
    
    X x;
    X& x1 = x;
    outer(x);
    outer(x1);
    outer(X());

    return 0;
}

int main(int argc, char *argv[])
{
    
    return 0;
}
