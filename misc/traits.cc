#include <iostream>

template<typename T>
struct is_bool
{
    static const bool type = false;
};

template<>
struct is_bool<bool>
{
    static const bool type = true;
};


template<typename T>
struct remove_reference
{
    typedef T value_type;

    static void func();
};

template<typename T>
void remove_reference<T>::func()
{
    std::cout << "normal version" <<std::endl;
}


template<typename T>
struct remove_reference<T&>
{
    typedef T value_type;

    static void func();
};


template<typename T>
void remove_reference<T&>::func()
{
    std::cout << "reference version" <<std::endl;
}

template<typename T>
struct remove_reference<T&&>
{
    typedef T value_type;
    static void func();
};

template<typename T>
void remove_reference<T&&>::func()
{
    std::cout << "right reference version" << std::endl;
}

template<typename T>
void test_remove_reference(T&& t)
{
    typename remove_reference<T>::value_type t1;
    remove_reference<T>::func();
    remove_reference<int&&>::func();

    t1  = t1;
}


int main(int argc, char *argv[])
{
    bool b;
   int i;
    std::cout <<is_bool<bool>::type <<std::endl;
    std::cout <<is_bool<int>::type <<std::endl;
    std::cout <<is_bool<int&&>::type <<std::endl;


    test_remove_reference(i);


    return 0;
}
