#ifndef SOURCE_H
#define SOURCE_H

#include <Frame.h>

class Source
{
public:
    Source();
    virtual ~Source();

    virtual int start();
    virtual int stop();

    //video attribute
    virtual int width() const {return 0;}
    virtual int height() const {return 0; }
    virtual PixelFormat pixelFormat() const {return PIXEL_FORMAT_NONE;}
    virtual Frame* getFrame();

};

#endif // SOURCE_H
