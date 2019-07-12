#ifndef _SKIPLIST_H
#define _SKIPLIST_H 

#include <iostream>
#include <utility>
#include <stddef.h>
#include <random>
#include <chrono>
#include <utils/utils.h>

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

template<typename K, typename T, unsigned char kMaxLevel>
SkipList<K, T, kMaxLevel>::SkipList()
    : generator(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count())
    , distribution(1, kMaxLevel)
{
    mHead = new node(K(), T());
    mHead->func();
    srand(time(NULL));
}

template<typename K, typename T, unsigned char kMaxLevel>
SkipList<K, T, kMaxLevel>::~SkipList()
{
    node* c = mHead->next[0];
    node* n;
    while (c) {
        n = c->next[0];
        delete c;
        c = n;
    }

    delete mHead;
}


template<typename K, typename T, unsigned char kMaxLevel>
bool SkipList<K, T, kMaxLevel>::insertNode(K key, T value)
{
    node* update[kMaxLevel] = {nullptr};
    node* c = nullptr; 
    unsigned char level = getLevel();

    for (int i = level-1; i >= 0; --i) {
        ++mSize[i];
    }

    for (int i = level-1; i >= 0; --i) {
        c = mHead;
        while (c->next[i]) {
            if (c->next[i]->key < key) {
                c = c->next[i];
            } else {
                break;
            }
        }
        update[i] = c;
    }

    node* n = new node(key, value, level);
    for (int i = level-1; i>=0; --i) {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }

    return true;
}

template<typename K, typename T, unsigned char kMaxLevel>
bool SkipList<K, T, kMaxLevel>::deleteNode(K key)
{
    node* update[kMaxLevel]{nullptr};
    node* c;

    for (int i = kMaxLevel-1; i >= 0; --i) {
        c = mHead;
        while (c->next[i]) {
            if (c->next[i]->key == key) {
                break;
            }
            c = c->next[i];
        }
        update[i] = c;
    }

next:
    if (!update[0])
        return false;

    node* d = update[0]->next[0];
    for (int i = d->level-1; i >= 0; --i) {
        update[i]->next[i] = d->next[i]; 
    }

    delete d;
    return true;
}

template<typename K, typename T, unsigned char kMaxLevel>
std::pair<bool, T> SkipList<K, T, kMaxLevel>::findNode(K key)
{
    for (int i = kMaxLevel-1; i >= 0; --i) {
        node* c = mHead->next[i];
        while (c) {
            if (c->key == key) {
                return {true, c->value};
            }
            c = c->next[i];
        }
    }

    return {false, T()};
}

template<typename K, typename T, unsigned char kMaxLevel>
void SkipList<K, T, kMaxLevel>::dump() const
{
    if (mSize[0] > 20) {
        for (int i = kMaxLevel-1; i >= 0; --i) {
            std::cout << "level " << i+1 << " size " << mSize[i] << std::endl;
        }
        return;
    }
    for (int i = kMaxLevel-1; i >= 0; --i) {
        node* c = mHead->next[i];
        std::cout << "level " << i+1 << " ==> ";
        while (c) {
            std::cout << "(" << c->key << " " << c->value << ") ";
            c = c->next[i];
        }
        std::cout << std::endl;
    }
}

template<typename K, typename T, unsigned char kMaxLevel>
void SkipList<K, T, kMaxLevel>::node::func()
{
    TRACE();
}


template<typename K, typename T, unsigned char kMaxLevel>
unsigned char SkipList<K, T, kMaxLevel>::getLevel()
{
    unsigned char level = 1;

    //while (generator()&1) {
        //++level;
    //}

    level = distribution(generator);

    return std::min<unsigned char>(level, kMaxLevel);
}


#endif /* ifndef _SKIPLIST_H */
