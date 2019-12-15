#include <SilentDream/Log.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

Log::LogLevel Log::sLogLevel  = LOG_LEVEL_VERBOSE;

int Log::initLogMode(SilentDream::RunMode runMode)
{
    if (runMode == SilentDream::DAEMON) {
        FILE* fp_log = fopen("/var/log/silentdream.log", "a");
        if (fp_log == NULL) {
            LOGE("open log file failed:%s\n", strerror(errno));
            return -1;
        }

        dup2(fileno(fp_log), 1);
        setbuf(stdout, NULL);

        fclose(fp_log);
    }

    return 0;
}

void Log::getCurrentTime(char* timebuf)
{
    struct timeval tv;
    struct tm t;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &t);
    sprintf(timebuf, "%02d-%02d %02d:%02d:%02d.%03lld", t.tm_mon+1, t.tm_mday,
            t.tm_hour, t.tm_min, t.tm_sec, tv.tv_usec/1000ll);
}

void Log::doLog(int level, const char* format, ...)
{
    if (level < sLogLevel) {
        return;
    }

    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}


