#ifndef _FIFO_H_
#define _FIFO_H_

#include <stddef.h>
#include <stdint.h>


struct fifo
{
public:
    fifo(size_t size);
    ~fifo();

    size_t get(void* buffer, size_t size);
    size_t put(const void* buffer, size_t size);
    size_t size() const;
    bool empty() const;

private:
    size_t mSize = 0;
    uint8_t* mBuffer = nullptr;
    size_t in = 0;
    size_t out = 0;
};



#endif
