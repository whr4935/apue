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

























struct node {
    int key;
    struct node *next;
};

void init_list(struct node **head);
void delete_next(struct node *head, struct node* t);
struct node* insert_after(int v, struct node* t);
void destroy_list(struct node *head);
void print_list(struct node* head);



#endif // LIST_H
