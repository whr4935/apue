#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <algorithm>
#include <vector>
#include <map>

class Poll;

using Callback = void(*)(Poll*, int, int);

struct Timepoint {
    void (*cb)(Timer *);
    Timer *timer;
    std::chrono::system_clock::time_point timepoint;
    int nextDelay;
};

class Loop
{
public:
    Loop() {
        mFd = epoll_create(1024);
        mTimePoint = std::chrono::system_clock::now();
    }

    virtual ~Loop() {
        ::close(mFd);
    }

    virtual void run();

    void addTimer(Timepoint t) {
        mTimers.insert(
            std::upper_bound(mTimers.begin(), mTimers.end(), t, [](const Timepoint&a, const Timepoint&b) {
                return a.timepoint < b.timepoint;
            }),
            t
        );

        ++mNumTimers;
        updateNextDelay();
    }

    void removeTimer(Timer* timer) {
        auto pos = mTimers.begin();
        for (Timepoint &t : mTimers) {
            if (t.timer == timer) {
                mTimers.erase(pos);
                break;
            }
            ++pos;
        }

        mCancelledLastTimer = true;
        --mNumTimers;
        updateNextDelay();
    }

    int add(int fd, epoll_event *event) {
        ++mNumPolls;
        return epoll_ctl(mFd, EPOLL_CTL_ADD, fd, event);
    }

    int del(int fd) {
        epoll_event e;

        --mNumPolls;
        return epoll_ctl(mFd, EPOLL_CTL_DEL, fd, &e);
    }

    int change(int fd, epoll_event *newEvent) {
        return epoll_ctl(mFd, EPOLL_CTL_MOD, fd, newEvent);
    }

    void setCb(int fd, Callback cb) {
        mCallbacks[fd] = cb;
    }

    void closeFd(Poll* poll, void(*cb)(Poll*)) {
        mClosing.push_back({poll, cb});
    }

private:
    std::chrono::system_clock::time_point now() {
        return mTimePoint = std::chrono::system_clock::now();
    }

    void updateNextDelay() {
        mDelay = -1;
        if (mTimers.size()) {
            mDelay = std::max<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                       mTimers[0].timepoint - mTimePoint), 0);
        }
    }

private:
    int mFd = -1;
    epoll_event mReadyEvents[1024];
    std::chrono::system_clock::time_point mTimePoint;
    int mDelay = -1;
    int mNumTimers = 0;
    std::vector<Timepoint> mTimers;
    bool mCancelledLastTimer = false;

    int mNumPolls = 0;
    std::map<int,Callback> mCallbacks;
    std::vector<std::pair<Poll*, void(*)(Poll*)>> mClosing;
};

class Timer {
public:
    Timer(Loop* loop)
        : mLoop(loop) {
    }

    virtual ~Timer() {
    }

    void start(void(*cb)(Timer*), int timeout, int repeat = 0) {
        std::chrono::system_clock::time_point timepoint = mLoop->now() +
                std::chrono::milliseconds(timeout);
        Timepoint t{cb, this, timepoint, repeat};
        mLoop->addTimer(t);
    }

    //stop() doesn't delete Timer object, we should do it ourself
    void stop() {
        mLoop->removeTimer(this);
    }

private:
    Loop* mLoop;
};


class Poll
{
public:
    Poll(Loop *loop, int fd)
        : mLoop(loop)
        , mFd(fd) {
    }

    virtual ~Poll() {
        if (mTimer) {
            cancelTimeout();
        }
    }

    virtual void start(int events) {
        epoll_event event;
        event.events = events;
        event.data.ptr = this;
        mLoop->add(mFd, &event);
    }

    virtual void stop() {
        mLoop->del(mFd);
    }

    virtual void change(int events) {
        epoll_event event;
        event.events = events;
        event.data.ptr = this;
        mLoop->change(mFd, &event);
    }

    virtual void setCb(Callback cb) {
        mLoop->setCb(mFd, cb);
    }

    virtual void startTimeout(void (*cb)(Timer*), int timeout) {
       mTimer = new Timer(mLoop);
       mTimer->start(cb, timeout);
    }

    virtual void cancelTimeout() {
        mTimer->stop();
        delete mTimer;
        mTimer = nullptr;
    }

    virtual int write(char* buf, size_t size);

    virtual void close(void(*cb)(Poll*)) {
        mFd = -1;
        mLoop->closeFd(this, cb);
    }

    bool isClosed() {
        return mFd == -1;
    }

    int fd() const {
        return mFd;
    }

protected:
    Loop *mLoop;
    int mFd;
    Timer* mTimer;
};

class Async : public Poll {
public:
    Async(Loop* loop)
        : Poll(loop, ::eventfd(0, EFD_CLOEXEC)) {
    }

    void start(void (*cb)(Async*)) {
        this->cb = cb;
        Poll::setCb([](Poll* p, int, int) {
            uint64_t val;
            if (::read(mFd, &val, 8) == 8) {
               ((Async*)p)->cb((Async*)p);
            }
        });
        Poll::start(EPOLLIN);
    }

    void notify() {
        int64_t one = 1;
        if (::write(mFd, &one, 8) != 8) {
            return;
        }
    }

    void close() {
        Poll::stop();
        ::close(mFd);
        Poll::close([](Poll *p) {
            delete p;
        });
    }

    void setData(void *data) {
        mData = data;
    }

    void* getData() const {
        return mData;
    }

private:
    void (*cb)(Async*);
    void* mData;
};


#endif // EPOLL_H
