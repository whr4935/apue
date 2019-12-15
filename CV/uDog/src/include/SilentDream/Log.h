#ifndef _LOG_H
#define _LOG_H

#include <sys/syscall.h>  

#include "SilentDream.h"

class Log {
public:
    enum LogLevel {
        LOG_LEVEL_VERBOSE,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERR,
    };

    static int initLogMode(SilentDream::RunMode runMode);
    static void getCurrentTime(char *timebuf);
    static void doLog(int level, const char* format,...);

private:
    static LogLevel sLogLevel;
};

#define gettid()  syscall(__NR_gettid)

#define STRINGIFY(s)         TOSTRING(s)
#define TOSTRING(s) #s

#ifndef LOG_TAG
#define LOG_TAG __FILE__ ":" STRINGIFY(__LINE__) 
#endif

#define LOG(level, levelStr, format, ...) \
    do { \
        char timebuf[30]; \
        Log::getCurrentTime(timebuf); \
        Log::doLog(level, "%s %d %d %s %s " format "\n", timebuf, getpid(), gettid(), levelStr, LOG_TAG, ##__VA_ARGS__); \
    } while (0)


#define LOGV(format,...) LOG(Log::LOG_LEVEL_VERBOSE, TOSTRING(V), format, ##__VA_ARGS__)
#define LOGI(format,...) LOG(Log::LOG_LEVEL_INFO, TOSTRING(I), format, ##__VA_ARGS__)
#define LOGW(format,...) LOG(Log::LOG_LEVEL_WARN, TOSTRING(W), format, ##__VA_ARGS__)
#define LOGE(format,...) LOG(Log::LOG_LEVEL_ERR, TOSTRING(E), format, ##__VA_ARGS__)


#endif
