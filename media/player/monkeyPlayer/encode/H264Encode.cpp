#include "H264Encode.h"

H264Encode::H264Encode()
{

}

H264Encode::~H264Encode()
{

}

int H264Encode::start(Source *source)
{
    if (mSource == nullptr) {
        mSource = source;
    }

    mSource->start();

    return 0;
}

int H264Encode::stop()
{
    if (mSource != nullptr) {
        mSource->stop();
    }

    return 0;
}

void* H264Encode::onMessageReceived(void *arg)
{
    H264Encode::H264Message* p = (H264Encode::H264Message*)arg;
    H264Encode* encode = p->mEncode;

    switch (p->cmd) {
    case CMD_FETCH_RAW_FRAME: {
        encode->onFetchRawFrame();

        H264Encode::H264Message msg(encode, CMD_ENCODE_FRAME);
        encode->postMessage(msg);
    }
        break;

    case CMD_ENCODE_FRAME: {
        encode->onEncodeFrame();

        H264Encode::H264Message msg(encode, CMD_DRAIN_H264_FRAME);
        encode->postMessage(msg);
    }
        break;

    case CMD_DRAIN_H264_FRAME: {
        encode->onDrainEncodedFrame();
    }
        break;

    default:
        break;
    }

    return nullptr;
}

////////////////////////////////////////////////
int H264Encode::init()
{
    mPicIn = new x264_picture_t;
    mPicOut = new x264_picture_t;
    mParam = new x264_param_t;

    x264_param_default(mParam);
    mParam->i_width = mSource->width();
    mParam->i_height = mSource->height();
    mParam->i_csp = (int)mSource->pixelFormat();

    x264_param_apply_profile(mParam, x264_profile_names[5]);

    mHandler = x264_encoder_open(mParam);

    x264_picture_init(mPicOut);
    x264_picture_alloc(mPicIn, mParam->i_csp, mParam->i_width, mParam->i_height);


    return 0;
}

int H264Encode::uninit()
{
    return 0;
}


int H264Encode::onFetchRawFrame()
{
    Frame* frame = mSource->getFrame();
    mRawFrames.push_back(frame);

    return 0;
}

int H264Encode::onEncodeFrame()
{
    int ret;
    x264_nal_t* pNal = nullptr;
    int iNal = 0;

    list<Frame*>::iterator it = mRawFrames.begin();
    if (it == mRawFrames.end())
        return 0;
    Frame* rawFrame = *it;
    mRawFrames.pop_front();




    ret = x264_encoder_encode(mHandler, &pNal, &iNal, mPicIn, mPicOut);
    if (ret < 0) {
        std::cout << "x264 encode faild!" << std::endl;
    }

    H264Frame* frame = nullptr;
    for (int i = 0; i < iNal; ++i) {
        frame = new H264Frame(pNal->p_payload, pNal->i_payload);
        mEncodedFrams.push_back(frame);
    }

    return iNal;
}


int H264Encode::onDrainEncodedFrame()
{
    return 0;
}












