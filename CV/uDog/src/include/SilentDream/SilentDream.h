#ifndef _SILENT_DREAM_H
#define _SILENT_DREAM_H

#include <pthread.h>
#include <signal.h>


class ArgumentParser;
class Loop;


class SilentDream {
public:
    enum RunMode {
        DAEMON,
        CLIENT,
    };

    static SilentDream* instance();
    virtual ~SilentDream();

    int exec(ArgumentParser& argParser);


private:
    SilentDream();
    SilentDream(const SilentDream&);
    SilentDream& operator=(const SilentDream&);

    static SilentDream* self;

	Loop* mLoop;

private:
    int initSignalHandler();
    static void* signalThreadHandler(void *arg);

    int execDaemon();

    int daemonize();
    int checkRunning(); 



    pthread_t mSignalTid;
    sigset_t mSignalMask;
    sigset_t mOldSignalMask;
};


#endif
