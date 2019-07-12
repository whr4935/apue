#include <iostream>
#include <algorithm>
#include <string.h>
#include <atomic>
#include <cassert>
#include "fifo.h"

size_t round_up_two_power(size_t size)
{
    size--;

    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;

    size++;

    return size;
}

fifo::fifo(size_t size)
: mSize(size)
{
    if (mSize&(mSize-1)) {
        mSize = round_up_two_power(mSize);
    }
    assert((mSize&(mSize-1)) == 0);

    mBuffer = new uint8_t[mSize];

}

fifo::~fifo()
{
    delete [] mBuffer;
}

size_t fifo::get(void* buffer, size_t size)
{
    size_t len = std::min(size, in - out);

    std::atomic_thread_fence(std::memory_order_seq_cst);

    size_t l = std::min(len, mSize - (out&(mSize-1)));
    memcpy(buffer, mBuffer + (out&(mSize-1)), l);
    memcpy((uint8_t*)buffer+l, mBuffer, len-l);

    std::atomic_thread_fence(std::memory_order_release);

    out += len;
    return len;
}

size_t fifo::put(const void* buffer, size_t size)
{
    size_t len = std::min(size, mSize-in+out);

    std::atomic_thread_fence(std::memory_order_seq_cst);

    size_t l = std::min(len, mSize - (in&(mSize-1)));
    memcpy(mBuffer + (in&(mSize-1)), buffer, l);
    memcpy(mBuffer, (uint8_t*)buffer+l, len-l);

    std::atomic_thread_fence(std::memory_order_release);

    in += len;
    return len;
}

size_t fifo::size() const
{
    return in - out;
}

bool fifo::empty() const
{
    return in == out;
}
