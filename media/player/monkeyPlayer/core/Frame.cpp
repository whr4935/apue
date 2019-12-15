#include <includes.h>
#include <Frame.h>

Frame::Frame()
    : width(0)
    , height(0)
    , pixelFormat(PIXEL_FORMAT_NONE)
    , lineCount(0)
{
    for (uint i = 0; i < ARRAY_SIZE(pdata); ++i) {
        pdata[i] = nullptr;
        lineSize[i] = 0;
    }
}

Frame::~Frame()
{
    release();
}

void Frame::release()
{
    for (uint i = 0; i < ARRAY_SIZE(pdata); ++i) {
        if (pdata[i] && lineSize[i] > 0) {
            delete [] pdata[i];
            pdata[i] = nullptr;
            lineSize[i] = 0;
        }
    }
}
