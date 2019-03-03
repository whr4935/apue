/*************************************************************************
 > File Name: mquque.h
 > Created Time: 2018年04月08日 星期日 16时01分30秒
 ************************************************************************/

#ifndef _QUQUE_H
#define _QUQUE_H

template <typename T>
class Queue {
public:
    Queue(unsigned int size);
    ~Queue();

    bool put(T d);
    T get();
    unsigned int size() {
        return mIn - mOut;
    }

private:
    unsigned int round_up_two_power(unsigned int);

    T* mBuffer;
    unsigned int mSize;
    unsigned int mIn;
    unsigned int mOut;
};

template <typename T>
Queue<T>::Queue(unsigned int size)
{
    size = round_up_two_power(size);

    mBuffer = new T[size];
    mSize = size;
    mIn = 0;
    mOut = 0;
}

template <typename T>
Queue<T>::~Queue()
{
    delete [] mBuffer;
}

template <typename T>
bool Queue<T>::put(T d)
{
    if (size() >= mSize)
        return false;

    unsigned int index = mIn & (mSize-1);
    mBuffer[index] = d;
    ++mIn;

    return true;
}

template <typename T>
T Queue<T>::get()
{
    if (size() == 0) {
        throw std::logic_error("size is 0");
    }

    unsigned int index = mOut & (mSize-1);
    T d = mBuffer[index];
    ++mOut;

    return d;
}

template <typename T>
unsigned int Queue<T>::round_up_two_power(unsigned int size)
{
    if ((size&(size-1))==0)
        return size;

    unsigned int u = (unsigned int)(~(((unsigned int)~0)>>1));
    while (!(u&size)) {
        u >>= 1;
    }

    return u <<= 1;
}



#endif
