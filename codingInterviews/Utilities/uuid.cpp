/*************************************************************************
 > File Name: uuid.cpp
 > Created Time: 2018年03月30日 星期五 16时45分29秒
 ************************************************************************/

#include "uuid.h"
#include <sys/time.h>
#include <unistd.h>

UUID::Creator UUID::creator;

uint64_t UUID::getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t time = tv.tv_usec;
    time /= 1000;
    time += tv.tv_sec * 1000;

    return time;
}

UUID::UUID()
    : mEpoch(0)
    , mTime(getTime())
    , mMachine(0)
    , mSequence(0)
{

}

UUID::~UUID()
{

}

UUID& UUID::instance()
{
    static UUID uuid;
    creator.dummy();

    return uuid;
}

void UUID::setEpoch(uint64_t epoch)
{
    mEpoch = epoch;
}

void UUID::setMachine(int32_t machine)
{
    mMachine = machine;
}

//序列号就是一系列的自增id（多线程建议使用atomic），为了处理在同一毫秒内需要给多条消息分配id，
//若同一毫秒把序列号用完了，则“等待至下一毫秒”。

uint64_t UUID::waitNextMs()
{
    uint64_t t;
    do {
        t = getTime();
    } while (t <= mTime);

    return t;
}

int64_t UUID::id()
{
    int64_t value = 0;
    uint64_t time = getTime();

    if (mTime == time) {
        mSequence = (mSequence+1)&0xFFF;
        if (mSequence == 0) {
            mTime = waitNextMs();
        }
    } else {
        std::cout << "here" << std::endl;
        mSequence = 0;
    }

    value = (mTime-mEpoch) << 22;
    value |= (mMachine&0x3FF)<<12;
    value |= mSequence & 0xFFF;

    return value;
}


