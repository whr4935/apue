/*************************************************************************
 > File Name: Blob.h
 > Created Time: 2017年10月21日 星期六 22时57分50秒
 ************************************************************************/

#ifndef _BLOB_H
#define _BLOB_H

#include <c_plus_plus_primer.h>

template<typename T>
class Blob {
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    Blob();
    Blob(std::initializer_list<T> il);
    template <typename Tt> Blob(Tt b, Tt e);

    size_type size() const {return data->size();}
    bool empty() const {return data->empty();}
    void push_back(const T& t) {data->push_back(t);}
    void push_back(T&& t) { data->push_back(std::move(t));}
    void pop_back();

    T& back();
    T& operator[](size_type i);

    static size_t tr;


private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string &msg) const;
};


template<typename T>
Blob<T>::Blob() {
    std::cout << "Blob default constructor" << std::endl;
}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) {
    std::cout << "Blob initializer_list" << std::endl;
}

template<typename T>
template <typename Tt>
Blob<T>::Blob(Tt b, Tt e) : data(std::make_shared<std::vector<int>>(b, e)) {
    std::cout << "template class template funcition." << std::endl;
}


template <typename T>
void Blob<T>::check(size_type i, const std::string &msg) const
{
    if (i >= data->size()) 
        throw std::out_of_range(msg);
}


template<typename T>
size_t Blob<T>::tr = 0; 

// template<>
// class Blob<int> {
// public:
    // Blob() {
        // std::cout << "Blob init" << std::endl;
    // }
    // static size_t tr;

// };

template <typename T>
class BlobPtr {
public:
    BlobPtr() :curr(0) {}
    BlobPtr(Blob<T> &a, size_t sz = 0) :
        wptr(a.data), curr(sz) {}
    T& operator*() const {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }

    BlobPtr& operator++();
    BlobPtr& operator--();

private:
    std::shared_ptr<std::vector<T>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};

template <typename T, bool isTrue>
class Number {
public:
    Number(const T& v) : m_v(v) {}
    template <typename C>
    int print(C c);
private:
    T m_v;
};

class DebugDelete {
public:
    DebugDelete(std::ostream &s = std::cerr) : os(s) { }

    template <typename T> void operator()(T *p) const {
        os << "deleting unique_ptr" << std::endl; delete p;
    }

private:
    std::ostream &os;
};

template <typename T>
struct Foo {
    Foo(const T& t = T()) : mem(t) {}
    void Bar() {

    }

    T mem;
};

// template <>
// void Foo<int>::Bar() 
// {
    // std::cout << "Foo<int> Bar()" << std::endl;
// }


#endif
