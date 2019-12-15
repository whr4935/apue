#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <includes.h>
#include <pthread.h>
#include <mutex>

#define MAX_DELAY       0x7fffffffffffffffLL

class Message {
public:
    typedef void*(*Handler)(void*);

    Message(Handler handler = nullptr, void* ptr = nullptr, int64_t delayMs = MAX_DELAY);

    static void* dummyHandler(void*);

    Handler mHandler;
    void* mPtr;
     int64_t delayMs;
     int id;
};

class MessageQueue
{
public:
    MessageQueue();
    virtual ~MessageQueue();

    int postMessage(Message& msg);
    int64_t getMessage(Message& msg);

    int run();
    int runAsync();
    int stop();
    static int64_t getNowMs();

private:
    void makeTimeOut(struct timespec *tsp, int64_t ms) const;
    void postDummyMessage();

    static void* runAsyncWrapper(void*arg);

    int64_t mLastMs;
    std::list<Message> mList;
    pthread_mutex_t mMutex;
    pthread_cond_t mCond;

    pthread_t mTid;

    pthread_mutex_t mAsyncMutex;
    bool bRunning;
};

#endif // MESSAGEQUEUE_H
