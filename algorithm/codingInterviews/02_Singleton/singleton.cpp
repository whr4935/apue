/*************************************************************************
 > File Name: 02_Singleton/singleton.cpp
 > Created Time: 2018年03月26日 星期一 22时03分32秒
 ************************************************************************/

#include <iostream>
#include <thread>

//兼容c++11以前版本的单例模式
template <typename T>
class Singleton
{
public:
    static T* instance() {
        static T obj;

        helper.dummy();
        return &obj;
    }

    Singleton() = delete;

    template <typename T2>
    struct Base {
        Base() {
            d2 = 10;
        }

        static void print() ;
       

    public:
        T2 d2;
    };



private:

    struct Helper {
        Helper() {
            std::cout << "call me" << std::endl;
            Singleton<T>::instance();     
        }

        void dummy() {} //没有这个空函数，根本不会有helper对象
    };

    static Helper helper;
};

template <typename T>
typename Singleton<T>::Helper Singleton<T>::helper;


//check multi template
template <typename T>
template <typename T2>
void Singleton<T>::Base<T2>::print()
{
    std::cout << "hello base" << std::endl;
}



int main()
{
    std::cout << "hell main" << std::endl;
    int* d = Singleton<int>::instance();
    *d = 1234;

    Singleton<int>::Base<int>::print();
    


    std::thread th2([] {
        int* d2 = Singleton<int>::instance();
        std::cout << "d2:" << *d2 << std::endl;
    });

   th2.join(); 


    return 0;
}
