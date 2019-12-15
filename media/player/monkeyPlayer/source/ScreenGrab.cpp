#include <includes.h>
#include "ScreenGrab.h"

#include <sys/time.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>


class X11Frame : public Frame
{
public:
    X11Frame(XImage* image = nullptr)
        :mXImage(image) {}
    void release();

    XImage* mXImage;
};

void X11Frame::release()
{
    if (mXImage != nullptr) {
        XDestroyImage(mXImage);
        mXImage = nullptr;
    }
}

///////////////////////////////////////////////
ScreenGrab::ScreenGrab()
    : mDisplay(nullptr)
    , mWidth(0)
    , mHeight(0)
    , mPixelFormat(PIXEL_FORMAT_NONE)
{
    init();
}

ScreenGrab::~ScreenGrab()
{
    destroy();
}

int ScreenGrab::start()
{
    return 0;
}

int ScreenGrab::stop()
{
    return 0;
}

Frame* ScreenGrab::getFrame()
{
    X11Frame* frame = nullptr;

    XImage* image = getOneImage();
    frame = new X11Frame(image);
    frame->pixelFormat = getPixelFormatFromImage(image);
    frame->width = image->width;
    frame->height = image->height;
    frame->pdata[0] = (uint8_t*)image->data;
    frame->lineSize[0] = image->width * image->height * image->bits_per_pixel/8;
    frame->lineCount = 1;

    return frame;
}

int ScreenGrab::width() const
{
    return mWidth;
}

int ScreenGrab::height() const
{
    return mHeight;
}

PixelFormat ScreenGrab::pixelFormat() const
{
    return mPixelFormat;
}

int ScreenGrab::init()
{
    mDisplay = XOpenDisplay(NULL);
    if (mDisplay == nullptr) {
        std::cout << "XOpenDisplay failed!" << std::endl;
        return -1;
    }

    mRoot = DefaultRootWindow(mDisplay);
    XWindowAttributes attr;
    XGetWindowAttributes(mDisplay, mRoot, &attr);
    mWidth = attr.width;
    mHeight = attr.height;

    XImage* image = getOneImage();
    mPixelFormat = getPixelFormatFromImage(image);
    XDestroyImage(image);

    return 0;
}

int ScreenGrab::destroy()
{
    if (mDisplay != nullptr) {
        XCloseDisplay(mDisplay);
        mDisplay = nullptr;
    }

    return 0;
}

XImage* ScreenGrab::getOneImage()
{
    XImage* image;

    image = XGetImage(mDisplay, mRoot, 0, 0, mWidth, mHeight, AllPlanes, ZPixmap);
    if (image == nullptr) {
        std::cout << "XGetImage failed!" << std::endl;
    }

    return image;
}

PixelFormat ScreenGrab::getPixelFormatFromImage(XImage *image) const
{
    PixelFormat format = PIXEL_FORMAT_NONE;

    switch (image->bits_per_pixel) {
    case 24:
        if (image->red_mask   == 0xff0000 &&
            image->green_mask == 0x00ff00 &&
            image->blue_mask  == 0x0000ff) {
            format = PIXEL_FORMAT_BGR24;
        } else if (image->red_mask   == 0x0000ff &&
                   image->green_mask == 0x00ff00 &&
                   image->blue_mask  == 0xff0000) {
            format = PIXEL_FORMAT_RGB24;
        }
        break;

    case 32:
        if (image->red_mask   == 0xff0000 &&
            image->green_mask == 0x00ff00 &&
            image->blue_mask  == 0x0000ff ) {
            format = PIXEL_FORMAT_BGRA32;
        }
        break;

    default:
        std::cout << "we don't support this pixel format!" << std::endl;
        break;
    }

    return format;
}

int ScreenGrab::writePPM(std::string filename, XImage *image)
{
    std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
    int width = image->width;
    int height = image->height;

    if (!out) {
        std::cout << "create out file " << filename.c_str() << " failed!" << std::endl;
        goto exit;
    }

    out << "P6\n" << width << " " << height << "\n255\n";
    unsigned int pixel;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixel = XGetPixel(image, x, y);
            if (image->red_mask == 0xFF0000) {
                pixel = pixel>>16 | (pixel&0xFF00) | (pixel&0xFF)<<16;
            }
            out.write((char*)&pixel, 3);
        }
    }

exit:
    XDestroyImage(image);

    return 0;
}

static int writePPM(std::string filename, X11Frame* frame)
{
    std::ofstream out(filename, std::ios_base::out | std::ios_base::binary);
    if (!out.is_open()) {
        std::cout << "open file " << filename << " failed!" << std::endl;
        frame->release();
        return -1;
    }

    out << "P6\n" << frame->width << " " << frame->height << "\n255\n";

    uint8_t* p;
    char pixel[3];
    for (int j = 0; j < frame->height; ++j) {
        for (int i = 0; i < frame->width; ++i) {
            switch (frame->pixelFormat) {
            case PIXEL_FORMAT_BGR24: {
                p = frame->pdata[0] + (j * frame->width + i) * 3;
                pixel[0] = p[2];
                pixel[1] = p[1];
                pixel[2] = p[0];
            }
                break;

            case PIXEL_FORMAT_BGRA32: {
                p = frame->pdata[0] + (j * frame->width + i) * 4;
                pixel[0] = p[2];
                pixel[1] = p[1];
                pixel[2] = p[0];
            }
                break;

            default: {
                p = frame->pdata[0] + (j * frame->width + i) *3;
                pixel[0] = p[0];
                pixel[1] = p[1];
                pixel[2] = p[2];
            }
                break;
            }
            out.write(pixel, 3);
        }
    }

    frame->release();
    return 0;
}

int ScreenGrab::test()
{
    std::string filename = "./grab_";
    struct timeval val;
    gettimeofday(&val, NULL);
    struct tm* t = localtime(&val.tv_sec);
    char timebuf[20];
    strftime(timebuf, 20, "%y%m%d_%H%M%S", t);
    filename.append(timebuf);
    filename.append(".ppm");
    std::cout << filename << std::endl;

#if 0
    XImage* image = getOneImage();
    if (image == nullptr) {
        std::cout << "grab failed!" << std::endl;
        return -1;
    }
    writePPM(filename, image);
#else
    X11Frame* frame = static_cast<X11Frame*>(getFrame());

    ::writePPM(filename, frame);

#endif


    return 0;
}
