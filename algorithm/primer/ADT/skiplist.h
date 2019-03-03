#ifndef _SKIPLIST_H
#define _SKIPLIST_H 

#include <utility>
#include <stddef.h>
#include <random>

template<typename K, typename T, unsigned char kMaxLevel=4>
class SkipList
{
public:
    SkipList();
    ~SkipList();
    bool insertNode(K key, T v);
    bool deleteNode(K key);
    std::pair<bool,T> findNode(K key);

    void dump() const;

private:
    unsigned char getLevel();
    struct node {
        K key;
        T value;
        unsigned char level;
        node *next[kMaxLevel];

        void func();

        node(const K& _key, const T& _value, unsigned char _level = 1) : 
            key(_key), value(_value), level(_level), next{0} { }
    };
    
    node* mHead;
    size_t mSize[kMaxLevel];
    std::mt19937 generator;
    std::uniform_int_distribution<unsigned char> distribution;
};


#endif /* ifndef _SKIPLIST_H */
