#ifndef SURFACE_H_
#define SURFACE_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <string>
#include <list>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#ifdef __cplusplus
extern "C" {
#endif

class Surface
{
public:
    class Frame {
        friend Surface;
        uint8_t* mData = nullptr;
        size_t mSize = 0;
        int mWidth;
        int mHeight;
    public:
        Frame(int width, int height);
        ~Frame();
        uint8_t* data() const {return mData;}
        size_t size() const {return mSize;}
    };

    Surface(int width, int height, int pix_fmt = 0);
    virtual ~Surface();
    void start();
    void stop();
    Frame* dequeueFrame();
    void queueFrame(Frame* f);
private:
    void initEGL();
    int initCheck();

    int readShaderFile(const std::string& filename, std::string &buf);
    GLuint loadShader(GLenum type, const char* shadrSrc);

    Frame* acquireFrame();
    void releaseFrame(Frame* f);
    void draw(Frame* f);

    void loop();

    int mWidth;
    int mHeight;

    EGLDisplay mDisplay;
    EGLContext mContext;
    EGLSurface mSurface;

    bool playYuvMode = true;
    GLuint vertex_buffer; //for test mode
    GLuint yuv_buffer[2];
    GLuint programObject;

    GLuint id_y, id_u, id_v;  // 纹理id
    GLuint textureUniformY, textureUniformU, textureUniformV;

    std::atomic<int> mQuit;
    std::mutex mMutex;
    std::condition_variable mCond;
    std::thread *mRendererThread = nullptr;
    std::list<Frame*> mFreeFrames;
    std::list<Frame*> mFrames;
};

#ifdef __cplusplus
}
#endif

#endif /* ifndef SURFACE_H_ */
