#ifndef LIST_H
#define LIST_H

#include <cstddef>

template<typename T>
class list
{
    struct node;
public:
    struct iterator;

    list();
    ~list();
    iterator begin() { return iterator(mMiddle->next);}
    iterator end() { return iterator(mMiddle);}
    //insert before specified iterator
    iterator insert(const iterator& it, const T& data);
    iterator insert(const iterator& it, T&& data) noexcept;
    iterator insert(const iterator& it, iterator begin, iterator end);
    iterator erase(iterator it);
    iterator erase(iterator begin, iterator end);
    void clear();
    size_t size();

    struct iterator {
        friend list;
        iterator(node* pNode) : mNode(pNode) {}
        iterator(const iterator& rhs) : mNode(rhs.mNode) {}
        ~iterator() {}

        iterator& operator++() {
            mNode = mNode->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(mNode);
            mNode = mNode->next;
            return tmp;
        }

        iterator& operator--() {
            mNode = mNode->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp(mNode);
            mNode = mNode->next;
            return tmp;
        }

        T& operator*() {
            return mNode->data;
        }

        T* operator->() {
            return &mNode->data;
        }

        bool operator==(const iterator& rhs) {
            return mNode == rhs.mNode;
        }

        bool operator!=(const iterator& rhs) {
            return !(*this == rhs);
        }

    private:
        node* mNode;
    };



private:
    struct node {
        T data;
        node* prev;
        node* next;

        node(const T& d = T()) : data(d), prev(nullptr), next(nullptr){}
        node(const T&& d) : data(d), prev(nullptr), next(nullptr){}
    };

    node* mMiddle;
};


template<typename T>
list<T>::list()
{
    mMiddle = (node*)new unsigned char[sizeof(node)];
    mMiddle->next = mMiddle;
    mMiddle->prev = mMiddle;
}

template<typename T>
list<T>::~list()
{
    TRACE();
    clear();
    delete[] (unsigned char*)mMiddle;
}

template<typename T>
typename list<T>::iterator list<T>::insert(const iterator& it, const T&data)
{
    node* c = new node(data);
    std::cout << "new node<T>: " << &c->data << std::endl;

    c->next = it.mNode;
    c->prev = it.mNode->prev;
    it.mNode->prev->next = c;
    it.mNode->prev = c;

    return iterator(c);
}

template<typename T>
typename list<T>::iterator list<T>::insert(const iterator& it, T&& data) noexcept
{
    node* c = new node(data);
    std::cout << "new node<T>: " << c->data << std::endl;
    c->next = it.mNode;
    c->prev = it.mNode->prev;
    it.mNode->prev->next = c;
    it.mNode->prev = c;

    return iterator(c);
}

template<typename T>
typename list<T>::iterator list<T>::insert(const iterator& it, iterator begin, iterator end)
{
    for (; begin != end; ++begin) {
        insert(it, *begin);
    }
}

template<typename T>
typename list<T>::iterator list<T>::erase(iterator it)
{
    node* c = it.mNode;
    node* p = c->prev;
    node* n = c->next;
    p->next = n;
    n->prev = p;
    delete c;

    return iterator(n);
}

template<typename T>
typename list<T>::iterator list<T>::erase(iterator begin, iterator end)
{
    while (begin != end) {
        erase(begin++);
    }

    return iterator(end);
}

template<typename T>
void list<T>::clear()
{
    node* c = mMiddle->next;
    node* n;

    while (c != mMiddle) {
        n = c->next;
        delete c;
        c = n;
    }

    mMiddle->next = mMiddle;
    mMiddle->prev = mMiddle;
}

template<typename T>
size_t list<T>::size()
{
    size_t count = 0;
    iterator it = begin();

    while (it != end()) {
        ++count;
        ++it;
    }

    return count;
} 




#endif // LIST_H
