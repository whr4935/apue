#ifndef H264ENCODE_H
#define H264ENCODE_H

#include <includes.h>
#include <Frame.h>
#include <MessageQueue.h>
#include <Source.h>

#include <x264.h>

class H264Frame : public Frame{
public:
    H264Frame(uint8_t* data, size_t size);
};

H264Frame::H264Frame(uint8_t* data, size_t size)
{
    this->pdata[0] = data;
    this->lineSize[0] = size;
}

/////////////////////////////////////////////////////
class H264Encode : public MessageQueue
{
public:
    enum {
        CMD_FETCH_RAW_FRAME,
        CMD_ENCODE_FRAME,
        CMD_DRAIN_H264_FRAME,
    };

    struct H264Message : public Message {
        H264Message(H264Encode* encode, int command)
            :mEncode(encode)
            ,cmd(command) {
        }

        H264Encode *mEncode;
        int cmd;
        void* opaque;
    };

    H264Encode();
    ~H264Encode();

    int start(Source* source);
    int stop();

    static void* onMessageReceived(void* arg);

private:
    friend struct H264Message;

    int init();
    int uninit();

    int onFetchRawFrame();
    int onEncodeFrame();
    int onDrainEncodedFrame();

    Source* mSource;
    list<Frame*> mRawFrames;

    x264_nal_t* mNal;
    x264_t* mHandler;
    x264_picture_t* mPicIn;
    x264_picture_t* mPicOut;
    x264_param_t* mParam;

    list<H264Frame*> mEncodedFrams;
};

#endif // H264ENCODE_H
