#include "skiplist.h"
#include <utils.h>
#include <time.h>
#include <algorithm>
#include <chrono>

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


int test_skipList()
{
    TRACE();
    SkipList<int, int> *s = new SkipList<int, int>();

    BenchMark b;
    benchmark_begin(&b);
    for (int i = 0; i < 10; ++i) {
        s->insertNode(i, i);
    }
    benchmark_end(&b, "insertNode");
    //std::cout << "insert OK" << std::endl;

    auto r = s->findNode(8);
    std::cout << "findNode " << 8 << ", get " << r.first << " " << r.second << std::endl;
    s->dump();

    std::cout << "deleteNode test" << std::endl;
    bool r2 = s->deleteNode(8);
    std::cout << "deleteNode " << 8 << " return " << r2 << std::endl;

    s->dump();

    delete s;

    return 0;
}
AUTO_RUN(test_skipList);
