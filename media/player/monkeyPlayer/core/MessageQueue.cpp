#include <MessageQueue.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <cstring>

using std::list;

Message::Message(Handler handler, void* ptr, int64_t delayMs)
    :mHandler(handler) 
    ,mPtr(ptr)
    ,delayMs(delayMs)
    ,id(0)
{

}

void* Message::dummyHandler(void* p)
{
    Message* msg = (Message*)p;

    std::cout << "id: " << msg->id << " ," << time(NULL) << "s" << std::endl;

    return nullptr;
}

//////////////////////////////////////////////////////
MessageQueue::MessageQueue()
    :mLastMs(MAX_DELAY)
    ,mTid(-1)
    ,bRunning(false)
{
    pthread_mutex_init(&mMutex, nullptr);
    pthread_cond_init(&mCond, nullptr);

    pthread_mutex_init(&mAsyncMutex, nullptr);
}

MessageQueue::~MessageQueue()
{
    pthread_mutex_destroy(&mAsyncMutex);

    pthread_mutex_destroy(&mMutex);
    pthread_cond_destroy(&mCond);
}

int MessageQueue::postMessage(Message &msg)
{
    pthread_mutex_lock(&mMutex);

    int64_t nowMs = getNowMs();
    int64_t deltaMs = 0;
    if (mLastMs != MAX_DELAY) {
        deltaMs = nowMs - mLastMs;
    }

    if (msg.delayMs != MAX_DELAY) {
        msg.delayMs += deltaMs;

        list<Message>::iterator it =  mList.begin();
        while (it != mList.end() && msg.delayMs >= it->delayMs) {
            msg.delayMs -= it->delayMs;
            ++it;
        }

        if (it != mList.end())
            it->delayMs -= msg.delayMs;
        mList.insert(it, msg);
    } else {
        msg.delayMs = 0LL;
        mList.push_back(msg);
    }

    if (mLastMs == MAX_DELAY) {
        mLastMs = nowMs;
    }

    pthread_mutex_unlock(&mMutex);

    return 0;
}

int64_t MessageQueue::getMessage(Message &msg)
{
    pthread_mutex_lock(&mMutex);

    int64_t nowMs = getNowMs();
    int64_t deltaMs = 0;
    if (mLastMs != MAX_DELAY) {
        deltaMs = nowMs - mLastMs;
    }

    int64_t timeOutMs = -1LL;
    int64_t nexDelayMs = MAX_DELAY;
    list<Message>::iterator it = mList.begin();
    if (it != mList.end()) {
        timeOutMs = deltaMs - it->delayMs;
    }

    if (timeOutMs < 0) {
        nexDelayMs = -timeOutMs;
        goto exit;
    }

    if (it != mList.end()) {
        msg = *it;
        mList.pop_front();
    }

    it = mList.begin();
    if (it != mList.end()) {
        it->delayMs -= timeOutMs;
        nexDelayMs = it->delayMs;
        mLastMs = nowMs;
    } else {
        mLastMs = MAX_DELAY;
        nexDelayMs = MAX_DELAY;
    }

exit:
    pthread_mutex_unlock(&mMutex);

    return nexDelayMs;
}

int64_t MessageQueue::getNowMs()
{
    int64_t nowMs;
    struct timeval tv;

    gettimeofday(&tv, nullptr);
    nowMs = tv.tv_sec*1000 + (tv.tv_usec+500)/1000;

    return nowMs;
}

void MessageQueue::makeTimeOut(timespec *tsp, int64_t ms) const
{
    struct timeval now;

    gettimeofday(&now, NULL);
    tsp->tv_sec = now.tv_sec;
    tsp->tv_nsec = now.tv_usec * 1000;

    tsp->tv_nsec += ms * 1000000;
    if (tsp->tv_nsec >= 1000000000LL) {
        tsp->tv_nsec -= 1000000000LL;
        tsp->tv_sec += 1;
    }
}

#define MESSAGE_QUEUE_TICK 50*1000

int MessageQueue::run()
{
    postDummyMessage();

    pthread_mutex_lock(&mAsyncMutex);
    bRunning = true;
    pthread_mutex_unlock(&mAsyncMutex);

    for (;;) {
        int64_t nextDelayMs;
        Message msg;

        nextDelayMs = getMessage(msg);
        if (msg.mHandler != nullptr) {
            (*msg.mHandler)(&msg);
        }

        if (nextDelayMs > 0) {
            if (nextDelayMs == MAX_DELAY) {
                nextDelayMs = MESSAGE_QUEUE_TICK;
            }
#if 0
            usleep(nextDelayMs);
#else
            struct timespec tsp;
            makeTimeOut(&tsp, nextDelayMs);
            pthread_cond_timedwait(&mCond, &mMutex, &tsp);
#endif
            pthread_mutex_unlock(&mMutex);
        }
    }

    pthread_mutex_lock(&mAsyncMutex);
    bRunning = false;
    pthread_mutex_unlock(&mAsyncMutex);

    return 0;
}

void* MessageQueue::runAsyncWrapper(void *arg)
{
    MessageQueue* q = (MessageQueue*)arg;

    q->run();

    return nullptr;
}

int MessageQueue::runAsync()
{

    pthread_mutex_lock(&mAsyncMutex);
     bool isRun =bRunning;
    pthread_mutex_unlock(&mAsyncMutex);

    if (isRun) {
        std::cout << "MessageQueue is running!" << std::endl;
        return 0;
    }

    int err = pthread_create(&mTid, nullptr, runAsyncWrapper, this);
    if (err != 0) {
        std::cout << "pthread_create failed:" << strerror(err) << std::endl;
        return -1;
    }

    return 0;
}

int MessageQueue::stop()
{
    pthread_mutex_lock(&mAsyncMutex);
     bool isRun =bRunning;
    pthread_mutex_unlock(&mAsyncMutex);

    if (!isRun) {
        std::cout << "MessageQueue isn't run!" << std::endl;
        return 0;
    }

    pthread_cancel(mTid);
    pthread_join(mTid, nullptr);

    return 0;
}

void MessageQueue::postDummyMessage()
{
    int64_t times[] = {
      1, 3, 2, 0, 9, 5
    };

    std::cout << "begin time:" << time(nullptr) << std::endl;

    for (unsigned int i = 0; i < sizeof(times)/sizeof(*times); ++i) {
        Message msg(Message::dummyHandler, nullptr, times[i]*1000);
        msg.id = i+1;
        postMessage(msg);
    }
}

