#ifndef _SCREEN_GRAB_H
#define _SCREEN_GRAB_H

#include <Source.h>
#include <Frame.h>

#include <X11/Xlib.h>
#include <vector>
#include <string>

class ScreenGrab : public Source{
public:
    ScreenGrab();
    ~ScreenGrab();

    int start();
    int stop();
    Frame* getFrame();

    int width() const;
    int height() const;
    PixelFormat pixelFormat() const;

    int test();
private:
    int init();
    int destroy();
    XImage* getOneImage();

    PixelFormat getPixelFormatFromImage(XImage* image) const;
    int writePPM(std::string filename, XImage* image);

    Display *mDisplay;
    Window mRoot;
    int mWidth;
    int mHeight;
    PixelFormat mPixelFormat;

    ScreenGrab(const ScreenGrab&);
    ScreenGrab& operator=(const ScreenGrab&);
};


#endif
