#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <SilentDream/SilentDream.h>
#include <SilentDream/Log.h>
#include "ArgumentParser.h"
#include "Epoll.h"


SilentDream* SilentDream::self = nullptr;
static pthread_once_t once_control = PTHREAD_ONCE_INIT;

SilentDream* SilentDream::instance()
{
    if (self == nullptr) {
        pthread_once(&once_control, [] {
                self = new SilentDream();
            }); 
    }

    return self;
}

SilentDream::SilentDream()
    : mLoop(new Loop())
{

}

SilentDream::~SilentDream()
{

}

int SilentDream::exec(ArgumentParser& argParser)
{

    if (argParser.getRunMode() == DAEMON) {
        return execDaemon();
    }

    for (;;) {
        mLoop->run();
    }

    return 0;
}

//////////////////////////////////////////////
int SilentDream::initSignalHandler()
{
    sigfillset(&mSignalMask);

    int err;
    if ((err = pthread_sigmask(SIG_BLOCK, &mSignalMask, &mOldSignalMask)) != 0) {
        LOGE("pthread_sigmask failed:%s", strerror(err));
        return -1;
    }

    err = pthread_create(&mSignalTid, nullptr, signalThreadHandler, this);
    if (err != 0) {
        LOGE("create signal thread failed:%s", strerror(err));
        return -1;
    }


    return 0;
}

void* SilentDream::signalThreadHandler(void *arg)
{
    SilentDream* sd = static_cast<SilentDream*>(arg);

    int err, signo;

    LOGV("signal thread ready to run...");

    for (;;) {
        err = sigwait(&sd->mSignalMask, &signo);
        if (err != 0) {
            LOGE("sigwait failed:%s", strerror(err));
            break;
        }


        switch (signo) {
        case SIGINT:
            LOGI("hello SIGINT");
            break;

        case SIGQUIT:
            LOGI("hello SIGQUIT");
            break;

        case SIGTERM:
            LOGI("hello SIGTERM");
            break;

        case SIGSEGV:
            LOGI("hello SIGSEGV");
            break;

        case SIGCHLD:
            LOGI("hello SIGCHLD");
            break;


        default:
            LOGV("received signal:%d", signo);
            break;
        }
    }

exit:
    return (void*)0;
}


//////////////////////////////////////////////
int SilentDream::execDaemon()
{
    if (daemonize() < 0) {
        return -1;
    }

    if (checkRunning() != 0) {
        return -1;
    }

    initSignalHandler();

    LOGV("daemon ready to run...");
    for (;;) {

        sleep(1);
    }

    return 0;
}

int SilentDream::daemonize()
{
    umask(0);
    
    int pid;
    if ((pid = fork()) < 0) {
        LOGE("fork:%s", strerror(errno));
        return -1;
    } else if (pid != 0) {
        exit(0);
    }

    setsid();
    if ((pid = fork()) < 0) {
        LOGE("fork:%s", strerror(errno));
        return -1;
    } else if (pid != 0) {
        exit(0);
    }

    if (chdir("/") < 0) {
        LOGE("chdir:%s", strerror(errno));
        return -1;
    }

    return 0;
}

int SilentDream::checkRunning()
{
    int fd;

    if ((fd = open("/var/run/silentdream.pid", O_RDWR|O_CREAT, 0644)) < 0) {
        LOGE("create pid file failed:%s", strerror(errno));
        return -1;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    if (fcntl(fd, F_SETLK, &fl) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            LOGW("already running!");
            return 1;
        }

        LOGE("lockfile failed:%s", strerror(errno));
        return -1;
    }

    ftruncate(fd, 0);

    char buf[16];
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);

    return 0;
}





