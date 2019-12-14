/*************************************************************************
 > File Name: uuid.h
 > Created Time: 2018年03月30日 星期五 16时40分21秒
 ************************************************************************/

#ifndef _UUID_H
#define _UUID_H

#include <stdint.h>
#include <atomic>
#include <iostream>


class UUID
{
public:
    static UUID& instance(); 
    void setEpoch(uint64_t epoch);
    void setMachine(int32_t machine);
    int64_t id();

private:
    struct Creator {
        Creator() {
            std::cout << "creator" << std::endl;
            UUID::instance();
        }

        void dummy() { }
    };
    static Creator creator;

    UUID();
    ~UUID();

    uint64_t getTime();
    uint64_t waitNextMs();

    uint64_t mEpoch;
    std::atomic<uint64_t> mTime;
    int32_t mMachine;
    std::atomic<int32_t> mSequence;
};




#endif
