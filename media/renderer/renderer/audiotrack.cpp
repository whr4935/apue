#include "audiotrack.h"

//test play, read from /dev/urandom

AudioTrack::AudioTrack()
{
    int rc;
    int size;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;

    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        printf("unable to open pcm device:%s\n", snd_strerror(rc));
        return ;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        printf("unable to set hw parameters:%s\n", snd_strerror(rc));
        return ;
    }
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("frame size:%lu\n", frames);
    size = frames*4; //2 bytes/sample, 2 channels

    mHandle = handle;

    frameSize = 4;
    frameCount = frames*1024;
    bufferSize = frameCount * frameSize;
    mBuffer = new uint8_t[bufferSize];
}

AudioTrack::~AudioTrack()
{
    stop();
    delete[] mBuffer;
}

int AudioTrack::test()
{
    printf("ALSA library version:%s\n", SND_LIB_VERSION_STR);

    printf("\nPCM stream types:\n");
    for (int val = 0; val <= SND_PCM_STREAM_LAST; ++val) {
        printf("    %s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));
    }
    
    printf("\naccess types:\n");
    for (int val = 0; val <= SND_PCM_ACCESS_LAST; ++val) {
        printf("    %s\n", snd_pcm_access_name((snd_pcm_access_t)val));
    }

    printf("\nPCM formats:\n");
    for (int val = 0; val <= SND_PCM_FORMAT_LAST; ++val) {
        if (snd_pcm_format_name((snd_pcm_format_t)val) != NULL)
            printf("    %s(%s)\n",
                    snd_pcm_format_name((snd_pcm_format_t)val),
                    snd_pcm_format_description((snd_pcm_format_t)val));
    }

    printf("\nPCM subformats:\n");
    for (int val = 0; val <= SND_PCM_SUBFORMAT_LAST; ++val) {
        printf("    %s(%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val),
                snd_pcm_subformat_description((snd_pcm_subformat_t)val));
    }

    printf("\nPCM states:\n");
    for (int val = 0; val <= SND_PCM_STATE_LAST; ++val) {
        printf("    %s\n", snd_pcm_state_name((snd_pcm_state_t)val));
    }

    int rc;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t *params;
    unsigned int val, val2;
    int dir;
    snd_pcm_uframes_t frames;

    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0); 
    if (rc < 0) {
        printf("unable to open pcm device:%s\n", snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    snd_pcm_hw_params_set_channels(handle, params, 2);

    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        printf("unable to set hw parameters:%s\n", snd_strerror(rc));
        exit(1);
    }

    printf("\n\nPCM handle name: '%s'\n", snd_pcm_name(handle));

    printf("PCM state = %s\n", snd_pcm_state_name(snd_pcm_state(handle)));

    snd_pcm_hw_params_get_access(params, (snd_pcm_access_t*)&val);
    printf("access type = %s\n", snd_pcm_access_name((snd_pcm_access_t)val));

    snd_pcm_hw_params_get_format(params, (snd_pcm_format_t*)&val);
    printf("format=%s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val),
            snd_pcm_format_description((snd_pcm_format_t)val));
    snd_pcm_hw_params_get_subformat(params, (snd_pcm_subformat_t*)&val);
    printf("subformat = %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val),
            snd_pcm_subformat_description((snd_pcm_subformat_t)val));
    
    snd_pcm_hw_params_get_channels(params, &val);
    printf("channels = %d\n", val);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    printf("period time = %d us, dir = %d\n", val, dir);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("period size = %d frames\n", (int)frames);

    snd_pcm_hw_params_get_buffer_time(params, &val, &dir);
    printf("buffer time = %d us\n", val);

    snd_pcm_hw_params_get_buffer_size(params, (snd_pcm_uframes_t*)&val);
    printf("buffer size = %d frames\n", val);

    snd_pcm_hw_params_get_periods(params, &val, &dir);
    printf("periods per buffer = %d frames\n", val);

    snd_pcm_hw_params_get_rate_numden(params, &val, &val2);
    printf("exact rate = %d/%d bps\n", val, val2);

    val = snd_pcm_hw_params_get_sbits(params);
    printf("significants = %d\n", val);

    //snd_pcm_hw_params_get_tick_time(params, &val, &dir);
    //printf("tick time = %d us\n", val);

    val = snd_pcm_hw_params_is_batch(params);
    printf("is batch = %d\n", val);

    val = snd_pcm_hw_params_is_block_transfer(params);
    printf("is block transfer = %d\n", val);

    val = snd_pcm_hw_params_is_double(params);
    printf("is double = %d\n", val);

    val = snd_pcm_hw_params_is_half_duplex(params);
    printf("is half duplex = %d\n", val);

    val = snd_pcm_hw_params_is_joint_duplex(params);
    printf("is joint duplex = %d\n", val);

    val = snd_pcm_hw_params_can_overrange(params);
    printf("can overrange = %d\n", val);

    val = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("can mmap = %d\n", val);

    val = snd_pcm_hw_params_can_pause(params);
    printf("can pause = %d\n", val);

    val = snd_pcm_hw_params_can_resume(params);
    printf("can resume = %d\n", val);

    val = snd_pcm_hw_params_can_sync_start(params);
    printf("can sync start = %d\n", val);

    snd_pcm_close(handle);


    return 0;
}

int AudioTrack::play()
{
    long loops;
    int rc;
    int size;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;
    char* buffer;

    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        printf("unable to open pcm device:%s\n", snd_strerror(rc));
        return -1;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        printf("unable to set hw parameters:%s\n", snd_strerror(rc));
        return -1;
    }
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("frame size:%lu\n", frames);
    size = frames*4; //2 bytes/sample, 2 channels
    buffer = (char*)malloc(size);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    loops = 5000000 / val;
    while (loops > 0) {
        --loops;
        rc = read(0, buffer, size);
        if (rc == 0) {
            fprintf(stderr, "end of file on input\n");
            break;
        } else if (rc != size) {
            fprintf(stderr, "short read: read %d bytes\n", rc);
        }
        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            fprintf(stderr, "underrun occurred!\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei:%s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames!\n", rc);
        }
    }
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}

int AudioTrack::start()
{
    if (mAudioThread!=nullptr)
        return 0;

    mAudioThread = new std::thread([this] {
            this->loop();
            });

    return 0;
}

int AudioTrack::stop()
{
    if (mQuit)
        return 0;

    mQuit = 1;
    mAudioThread->join();
    delete mAudioThread;
    mAudioThread = nullptr;

    return 0;
}

size_t AudioTrack::write(u_int8_t* data, size_t size)
{
    Buffer buffer;
    int len;
    assert((size&(frameSize-1))==0);
    while (size > 0) {
        obtainBuffer(&buffer, size/frameSize); 
        //printf("write size:%lu, buffer.size:%lu\n", size, buffer.size);
        memcpy(buffer.buffer, data, buffer.size);
        data += buffer.size;
        size -= buffer.size;
        len += buffer.size;
        releaseBuffer(&buffer);
    }

    return len;
}

int AudioTrack::obtainBuffer(Buffer* buffer, size_t framesReq)
{
    std::unique_lock<std::mutex> _l(mMutex);
    mCond.wait(_l, [this] {
            return frameAvailable_l() > 0;
            });

    size_t frameAvail = frameAvailable_l();
    if (framesReq > frameAvail)
        framesReq = frameAvail;

    size_t u = user;
    size_t bufferEnd = userBase + frameCount;
    if (framesReq > bufferEnd - u) {
        framesReq = bufferEnd - u;
    }
    buffer->buffer = bufferPtr_l(); 
    buffer->frameCount = framesReq;
    buffer->size = framesReq * frameSize;

    return 0;
}

void AudioTrack::releaseBuffer(Buffer* buffer)
{
    std::unique_lock<std::mutex> _l(mMutex);
    stepUser_l(buffer->frameCount);
}

void AudioTrack::loop()
{
    int rc;
    for (;;) {
        mMutex.lock();
        size_t frameRd = frameReady_l();
        mMutex.unlock();

        if (frameRd == 0) {
            if (mQuit) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }

        mMutex.lock();
        uint8_t* buffer = mBuffer + (server-serverBase)*frameSize;
        size_t bufferEnd = serverBase + frameCount;
        if (frameRd > bufferEnd - server) {
            frameRd = bufferEnd - server;
        }
        mMutex.unlock();

        printf("audioThread read buffer:%p, frameRd:%lu\n", buffer, frameRd);
        rc = snd_pcm_writei(mHandle, buffer, frameRd);
        if (rc == -EPIPE) {
            fprintf(stderr, "underrun occurred!\n");
            snd_pcm_prepare(mHandle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei:%s\n", snd_strerror(rc));
        } else if (rc != (int)frameRd) {
            fprintf(stderr, "short write, write %d frames!\n", rc);
        }

        mMutex.lock();
        server += frameRd;
        if (server >= serverBase + frameCount) {
            serverBase += frameCount;
        }
        mCond.notify_one();
        mMutex.unlock();
    }

    snd_pcm_drain(mHandle);
    snd_pcm_close(mHandle);

    printf("audioThread exit...\n");
}
