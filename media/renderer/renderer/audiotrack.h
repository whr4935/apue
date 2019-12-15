#ifndef AUDIOTRACK_H_
#define AUDIOTRACK_H_

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class AudioTrack {
public:
    struct Buffer {
        u_int8_t* buffer;
        size_t frameCount;
        size_t size;
    };

    AudioTrack();
    ~AudioTrack();
    int start();
    int stop();
    size_t write(u_int8_t* data, size_t size);

    int play();

    static int test();
private:
    AudioTrack(const AudioTrack&);
    AudioTrack& operator=(const AudioTrack&);
    
    void stepUser_l(size_t size) {
        size_t u = user;
        u += size;
        if (u >= userBase + frameCount) {
            userBase += frameCount;
        }
        user = u;
    }

    void stepServer_l(size_t size) {
        size_t s = server;
        s += size;
        if (s >= serverBase + frameCount) {
            serverBase += frameCount;
        }
        server = s;
    }

    //empty frame space
    size_t frameAvailable_l() const {
        return frameCount - user + server; 
    }

    //writed frames
    size_t frameReady_l() const {
        return user - server;
    }

    u_int8_t* bufferPtr_l() const {
        return mBuffer + (user - userBase)*frameSize;
    }

    int obtainBuffer(Buffer* buffer, size_t framesReq);
    void releaseBuffer(Buffer*);

    void loop();

    size_t user = 0;
    size_t userBase = 0;
    size_t server = 0;
    size_t serverBase = 0;
    u_int8_t* mBuffer = nullptr;
    size_t bufferSize = 0;
    size_t frameCount = 0;
    size_t frameSize = 0;

    std::thread* mAudioThread = nullptr;
    std::mutex mMutex;
    std::condition_variable mCond;
    std::atomic<int> mQuit{0};

    snd_pcm_t* mHandle;
};


#endif /* ifndef AUDIOTRACK_H_ */
