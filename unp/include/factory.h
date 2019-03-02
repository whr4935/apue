#ifndef _FACTORY_H
#define _FACTORY_H

#include <iostream>
#include <string>
#include <map>

template<typename T>
class Factory {
public:
    int registerClass(std::string name, T* c);
    int unregisterClass(std::string name);
    T* getClass(std::string name);
    static Factory<T> *instance();

private:
    Factory();
    std::map<std::string, T*> mClasses;
    static Factory* sFactory;
};

template<typename T>
Factory<T>::Factory()
{

}

template<typename T>
int Factory<T>::registerClass(std::string name, T *c)
{
    auto p = std::make_pair(name, c);
    auto ret = mClasses.insert(p);

    return ret.second;
}

template<typename T>
int Factory<T>::unregisterClass(std::string name)
{
    int ret = mClasses.erase(name);

    return ret;
}

template<typename T>
T* Factory<T>::getClass(const std::string name)
{
    T* c = mClasses.at(name);

    return c;
}

template<typename T>
Factory<T>* Factory<T>::instance()
{
    if (sFactory == nullptr) {
        sFactory = new Factory<T>();
    }

    return sFactory;
}

template<typename T>
Factory<T>* Factory<T>::sFactory = nullptr;


#define TO_STRING(x)        #x

#define REGISTER_CLASS_AUTO(CLASS_NAME)    \
    namespace { \
    struct Register {    \
        Register() {   \
            std::cout << "register" << TO_STRING(CLASS_NAME) << std::endl;  \
            Factory<CLASS_NAME>::instance()->registerClass(TO_STRING(CLASS_NAME), new CLASS_NAME());    \
        }   \
        ~Register() {   \
            std::cout << "unregister" << TO_STRING(CLASS_NAME) << std::endl;  \
        }   \
    } s;    \
}



#endif
