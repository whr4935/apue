#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <stdlib.h>

enum PixelFormat{
    PIXEL_FORMAT_NONE                 = 0,
    PIXEL_FORMAT_RGB24                = 1,
    PIXEL_FORMAT_BGR24                = 2,
    PIXEL_FORMAT_BGRA32               = 3,
    PIXEL_FORMAT_YUV420P              = 4,
};

class Frame {
public:
    Frame();
    virtual void release();

    int width;
    int height;
    PixelFormat pixelFormat;
    uint8_t* pdata[4];
    size_t lineSize[4];
    int lineCount;

protected:
    virtual ~Frame();
};


#endif // FRAME_H
