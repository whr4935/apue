#include <iostream>
#include <utils.h>
#include "list.h"




////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////
struct dummy {
    dummy(int d) : mData(d) {
        std::cout << "dummy ctor: " << this << std::endl;
    }

    ~dummy() {
        std::cout << "dummy dtor: " << this << std::endl;
    }

    dummy operator++(int) {
        dummy tmp(mData);
        ++mData;
        return tmp;
    }

    dummy& operator++() {
        ++mData;
        return *this;
    }


    int mData;
};

std::ostream& operator<<(std::ostream&o, dummy& d) 
{
    o << d.mData;    
    return o;
}

void test1()
{

    list<dummy> l;
    dummy d(3);

    auto it = l.begin();
    for (int i = 0; i < 4; ++i) {
        it = l.insert(l.end(), ++d);
    }

    l.insert(l.begin(), l.begin(), l.end());

    //l.erase(++l.begin());

    //l.erase(l.begin(), --l.end());

    std::cout << "list size: " << l.size() << std::endl;
    for (auto &d : l) {
        std::cout << "get: " << d << std::endl;
    }

    std::cout << "leave test1" << std::endl;
}

///////////////////////////////////////////////////////
struct node {
    int key;
    struct node *next;
};

void init_list(struct node **head);
void delete_next(struct node *head, struct node* t);
struct node* insert_after(int v, struct node* t);
void destroy_list(struct node *head);
void print_list(struct node* head);

void calc_prime(int array[], int size)
{
    int i;
    int j;

    for (array[1] = 0, i = 2; i < size; ++i)
        array[i] = i;

    for (i = 2; i <= size/2; ++i) {
        if (array[i] == 0)
            continue;

        for (j = 2; j <=size/i; ++j) {
            array[i * j] = 0;
        }
    }
}

#define N 100

int test_calc_prime()
{
    static int data[N+1];

    calc_prime(data, ARRAY_SIZE(data));

    unsigned int i;
    int count = 0;
    for (i = 2; i < ARRAY_SIZE(data); ++i) {
        if (data[i]) {
            printf("%3d", data[i]);
            ++count;

            if (count%10 == 0)
                printf("\n");
        }
    }
    printf("\n");

    return 0;
}

////////////////////
void init_list(struct node **head)
{
    *head = (struct node*)malloc(sizeof(struct node));
    if (*head == NULL)
        printf("init list failed!\n");

    (*head)->next = *head;
}

void delete_next(struct node *head, struct node* t)
{
    struct node *tmp = t->next;

    if (tmp != head) {
        t->next = t->next->next;
        free(tmp);
    }
}

struct node* insert_after(int v, struct node* t)
{
    struct node* x;

    x = (struct node*)malloc(sizeof(struct node));
    x->key = v;
    x->next = t->next;
    t->next = x;

    return x;
}

void destroy_list(struct node *head)
{
    while (head->next != head)
        delete_next(head, head->next);

    free(head);
}

void print_list(struct node* head)
{
    struct node *t;

    t = head->next;
    printf("list:");
    while (t != head) {
        printf("%3d", t->key);
        t = t->next;
    }

    printf("\n");
}


int test_linked_list()
{
    struct node *head;

    init_list(&head);

    print_list(head);

    int i;

    for (i = 0; i < 10; ++i) {
        insert_after(i+1, head);
    }

    print_list(head);


    return 0;
}


void test_josephus()
{
    struct node *x, *t;
    int const total = 9;
    int const step = 1;

    t = (struct node*)malloc(sizeof(struct node));
    t->key = 1;
    x = t;

    int i;
    for (i = 2; i <= total; ++i) {
        t->next = (struct node*)malloc(sizeof(struct node));
        t = t->next;
        t->key = i;
    }

    t->next = x;

    printf("josephus begin:");

    while (t != t->next) {
        for (i = 1; i < step; ++i)
            t = t->next;
        printf("%3d ", t->next->key);
        x = t->next;
        t->next = x->next;
        free(x);
    }
    printf("%3d\n", t->key);


    printf("\n");
}

void test_josephus_v2()
{
    struct node *x, **pp;
    const int total = 9;
    const int step = 5;

    x= (struct node*)malloc(sizeof(struct node));
    x->key = 1;
    x->next = (struct node*)malloc(sizeof(struct node));
    x->next->next = x;
    x->next->key = total;

    int n = total;
    struct node *t;
    while (--n > 1) {
        t = (struct node*)malloc(sizeof(struct node));
        t->key = n;
        t->next = x->next;
        x->next = t;
    }

    printf("test_josephus_v2 begin:\n");
    printf("%d ", x->key);
    while (t != x) {
        printf("%d ", t->key);
        t = t->next;
    }
    printf("\n\n");

    printf("print result: ");
    pp = &x;
    while ((*pp)->next != *pp) {
        for (int i = 1; i < step; ++i) {
            pp = &(*pp)->next;
        }

        t = container_of(pp, struct node, next);
        /* printf("t->key = %d\n", t->next->key); */
        t = *pp;
        *pp = (*pp)->next;
        printf("%d ", t->key);
        free(t);
    }
    printf("%d\n", (*pp)->key);
}


/////////////////////////////////////////////////////
int list_test()
{
    BenchMark m;
    benchmark_begin(&m);

    test_calc_prime();

    test_linked_list();

    test_josephus();
    test_josephus_v2(); 

    test1();

    benchmark_end(&m, __func__);
    return 0;
}
AUTO_RUN(list_test);
 
