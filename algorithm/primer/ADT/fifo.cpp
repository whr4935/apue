#include <iostream>
#include <algorithm>
#include <string.h>
#include <atomic>
#include <cassert>
#include "fifo.h"

#define FIFO_VERSION_V1 1
#define FIFO_VERSION_V2 2 //in and out would rollback when reach max size

#define FIFO_VERSION FIFO_VERSION_V2

namespace alg {
static size_t round_up_power_of_two(size_t size)
{
    --size;

    for (size_t i = 1; i < sizeof(size_t)*8; i <<= 1) {
        size |= size >> i;
    }

    ++size;
    return size;
}

static size_t round_up_power_of_two_v2(size_t size)
{
    --size;

    while (size & size-1) {
        size = size & size-1;
    }

    size = size ? size<<1 : ++size;

    return size;
}

fifo::fifo(size_t size)
: mSize(size)
{
    printf("test: round_up_power_of_two_v2(3077) = %d\n", round_up_power_of_two_v2(3077));
    printf("test: round_up_power_of_two_v2(2) = %d\n", round_up_power_of_two_v2(2));
    printf("test: round_up_power_of_two_v2(1) = %d\n", round_up_power_of_two_v2(1));
    printf("test: round_up_power_of_two(1) = %d\n", round_up_power_of_two(1));

    if (mSize&(mSize-1)) {
        mSize = round_up_power_of_two(mSize);
        printf("sizeof(size_t)=%zu, round_up_power_of_two: %zu ==> %zu\n", sizeof(size_t), size, mSize);
    }
    assert((mSize&(mSize-1)) == 0);

    mBuffer = new uint8_t[mSize];

}

fifo::~fifo()
{
    delete [] mBuffer;
}

#if FIFO_VERSION == FIFO_VERSION_V1
size_t fifo::get(void* buffer, size_t size)
{
    size_t len = std::min(size, in - out);

    std::atomic_thread_fence(std::memory_order_acquire);

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

    std::atomic_thread_fence(std::memory_order_acquire);

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

#elif FIFO_VERSION == FIFO_VERSION_V2
size_t fifo::get(void* buffer, size_t size)
{
    size_t len = std::min(size, (in-out)&(mSize-1));

    std::atomic_thread_fence(std::memory_order_seq_cst);

    size_t l = std::min(len, mSize - out);
    memcpy(buffer, mBuffer + out, l);
    memcpy((uint8_t*)buffer+l, mBuffer, len-l);

    std::atomic_thread_fence(std::memory_order_release);

    out = (out + len) & (mSize-1);
    return len;
}

size_t fifo::put(const void* buffer, size_t size)
{
    size_t len = std::min(size, (out-in-1)&(mSize-1));

    std::atomic_thread_fence(std::memory_order_seq_cst);

    size_t l = std::min(len, mSize - in);
    mempcpy(mBuffer + in, buffer, l);
    memcpy(mBuffer, (uint8_t*)buffer+l, len-l);

    std::atomic_thread_fence(std::memory_order_release);

    in = (in + len) & (mSize-1);
    return len;
}

size_t fifo::size() const
{
    return (in - out) & (mSize-1);
}

//left one space
bool fifo::empty() const
{
    return (out-in-1) & (mSize-1);
}


#else
#error "Wrong fifo version!"
#endif


}
