#include <iostream>
#include <cstring>
#include <cstdarg>
#include <fstream>

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "surface.h"

#define die(s,...) \
        ({ \
         printf("Error line %d: " s "\n", __LINE__, ##__VA_ARGS__); \
         exit(-1);})

static Display* x_display = nullptr;

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

EGLNativeWindowType creatXWindow()
{
    x_display = XOpenDisplay(NULL);
    if (x_display == NULL) {
        die("XOpenDisplay");
    }

    Window root = DefaultRootWindow(x_display);
    XSetWindowAttributes swa, xattr;
    swa.event_mask = ExposureMask|PointerMotionMask|KeyPressMask;
    Window win = XCreateWindow(x_display, root, 0, 0, 640, 480, 0,
            CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);
    xattr.override_redirect = false;
    XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

    XWMHints hints;
    hints.input = true;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    XMapWindow(x_display, win);
    XStoreName(x_display, win, "renderer");

    Atom wm_delete_window = XInternAtom(x_display, "VM_DELETE_WINDOW", false);
    XSetWMProtocols(x_display, win, &wm_delete_window, 1);

    Atom wm_state;
    wm_state = XInternAtom(x_display, "_NET_WM_STATE", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = false;
    XSendEvent(x_display, DefaultRootWindow(x_display),
            false,SubstructureNotifyMask,&xev);
    
    return (EGLNativeWindowType)win;
}

bool userInterrupt()
{
    XEvent xev;
    KeySym key;
    bool userInterrupt = false;
    char text;

    while (XPending(x_display)) {
        XNextEvent(x_display, &xev);
        if (xev.type == KeyPress) {
            if (XLookupString(&xev.xkey, &text, 1, &key, 0)==1) {
            }
        }

        if (xev.type == DestroyNotify) {
            userInterrupt = true;
        } else if (xev.type == ClientMessage) {
            break;
        }
    }

    return userInterrupt;
}

///////////////////////////////////////////////////
Surface::Frame::Frame(int width, int height)
    : mWidth(width)
    , mHeight(height)
    , mSize(width*height*3/2)
{
    mData = new uint8_t[mSize];
}

Surface::Frame::~Frame()
{
    delete[] mData;
}

Surface::Surface(int width, int height, int pix_fmt)
    : mQuit(0)
    , mWidth(width)
    , mHeight(height)
{

}

Surface::~Surface()
{
    stop();
}

void Surface::start()
{
    if (mRendererThread != nullptr) {
        printf("Renderer already started!\n");
        return;
    }

    mRendererThread = new std::thread([this]() {
            this->loop();
            }); 
}

void Surface::stop()
{
    if (mRendererThread != nullptr) {
        mQuit = 1;
        mRendererThread->join();
        mRendererThread = nullptr;
    }
}

Surface::Frame* Surface::dequeueFrame()
{
    if(mQuit || mRendererThread==nullptr) {
        //printf("mRendererThread:%p\n",  mRendererThread);
        return nullptr;
    }

    std::unique_lock<std::mutex> _l(mMutex);
    //printf("mFreeFrames.size():%lu\n", mFreeFrames.size());
    auto ret = mCond.wait_for(_l, std::chrono::microseconds(10), [this] {
        return this->mFreeFrames.size()||mQuit;
            });

    if (!ret || mQuit) return nullptr;
    Frame* f = *mFreeFrames.begin();
    mFreeFrames.pop_front();
    return f;
}

void Surface::queueFrame(Frame* f)
{
    if (mQuit || mRendererThread==nullptr)
        return;

    std::lock_guard<std::mutex> _l(mMutex);
    mFrames.push_back(f);
}

////////////////////////////////////////////////
void Surface::initEGL()
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        die("eglGetDisplay");
    }

    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(display, &majorVersion, &minorVersion)) {
        die("eglInitialize");
    }
    std::cout << "majorVersion:" << majorVersion << std::endl;
    std::cout << "minorVersion:" << minorVersion << std::endl;
    
    EGLint numConfigs;
    if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
        die("eglGetConfigs");
    }
    std::cout << "numConfigs:" << numConfigs << std::endl;

    EGLNativeWindowType win = creatXWindow();

    EGLint attriblist[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_STENCIL_SIZE, EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_NONE
    };
    EGLConfig config;
    if (!eglChooseConfig(display, attriblist, &config, 1, &numConfigs)) {
        die("eglChooseConfig");
    }

    EGLSurface surface;
    surface = eglCreateWindowSurface(display, config, win, NULL);
    if (surface == EGL_NO_SURFACE) {
        die("eglCreateWindowSurface %#x", eglGetError());
    }

    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        die("eglCreateContext");
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        die("eglMakeCurrent %#x", eglGetError());
    }

    mDisplay = display;
    mContext = context;
    mSurface = surface;
}

int Surface::initCheck()
{
    std::string buf;
    std::string path = getenv("PWD");
    path += "/mbox/renderer";
    if (!playYuvMode) 
        readShaderFile(path+"/hello.vsh", buf);
    else
        readShaderFile(path+"/yuv420.vert", buf);
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, buf.c_str());
    buf.clear();
    if (!playYuvMode)
        readShaderFile(path+"/hello.fsh", buf);
    else
        readShaderFile(path+"/yuv420.frag", buf);
    GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, buf.c_str());

    programObject = glCreateProgram();
    if (programObject == 0)
        die();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragShader);
    glBindAttribLocation(programObject, ATTRIB_VERTEX, "position");
    //std::cout << __LINE__ << " " << glGetError() << std::endl;
    if (playYuvMode) {
        glBindAttribLocation(programObject, ATTRIB_TEXTURE, "TexCoordIn");
        //std::cout << __LINE__ << " " << glGetError() << std::endl;
    }

    glLinkProgram(programObject);

    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = (char*)malloc(infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            printf("Error linking program:%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
    }

    static GLfloat vVertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    static const GLfloat squareVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
    static const GLfloat coordVertices[] = {
        0.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };

    if (!playYuvMode) {
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    } else {
        glGenBuffers(2, yuv_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, yuv_buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, yuv_buffer[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coordVertices), coordVertices, GL_STATIC_DRAW);

        textureUniformY = glGetUniformLocation(programObject, "tex_y");
        textureUniformU = glGetUniformLocation(programObject, "tex_u");
        textureUniformV = glGetUniformLocation(programObject, "tex_v");

        glGenTextures(1, &id_y);
        glBindTexture(GL_TEXTURE_2D, id_y);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &id_u);
        glBindTexture(GL_TEXTURE_2D, id_u);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &id_v);
        glBindTexture(GL_TEXTURE_2D, id_v);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glUseProgram(programObject);
    }

    return 0;
}

int Surface::readShaderFile(const std::string& filename, std::string &buf)
{
    std::fstream in(filename);
    if (!in.is_open()) {
        die("open %s", filename.c_str());
    }
    in.seekg(0, std::ios_base::end);
    auto size = in.tellg();
    std::cout << "size:" << size << std::endl;
    buf.resize(size, '\0');
    in.seekg(0);
    in.read(&buf[0], size);

    return 0;
}

GLuint Surface::loadShader(GLenum type, const char*shaderSrc)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = (char*)malloc(infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("Error compiling shader:%s", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

Surface::Frame* Surface::acquireFrame()
{
    Frame *f = nullptr;

    std::lock_guard<std::mutex> _l(mMutex);
    if (mFrames.size() > 0) {
        f = *mFrames.begin();
        mFrames.pop_front();
    }

    return f;
}

void Surface::releaseFrame(Frame* f)
{
    std::unique_lock<std::mutex> _l(mMutex);
    bool needNotify = mFreeFrames.empty();
    mFreeFrames.push_back(f);
    if (needNotify) mCond.notify_one();
}

void Surface::draw(Frame* f)
{
    if (!playYuvMode) {
        glViewport(0, 0, 640, 480);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        return;
    }

    int w = f->mWidth;
    int h = f->mHeight;
    uint8_t* plane[3];
    plane[0] = f->data();
    plane[1] = f->data() + w * h;
    plane[2] = plane[1] + w*h/4;
    
    glBindBuffer(GL_ARRAY_BUFFER, yuv_buffer[0]);
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    //std::cout << glGetError() << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, yuv_buffer[1]);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);
    //std::cout << glGetError() << std::endl;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w/2, h/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w/2, h/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[2]);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glUniform1i(textureUniformY, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glUniform1i(textureUniformU, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glUniform1i(textureUniformV, 2);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return;
}

void Surface::loop()
{
    int ret = 0;
    printf("begin renderer thread...\n");
    initEGL();
    ret = initCheck();
    if (ret < 0) {
        printf("renderer thread initCheck failed!\n");
        return ;
    }

    Frame *f = nullptr;
    {
        std::unique_lock<std::mutex> _l(mMutex);
        for (int i = 0; i < 3; ++i) {
            f = new Frame(mWidth, mHeight);
            mFreeFrames.push_back(f);
        }
        mCond.notify_one();
    }

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        if (userInterrupt()) {
            printf("userInterrupt!\n");
            break;
        }

        f = acquireFrame();
        if (f==nullptr && mQuit)
            break;

        if (f) {
            //printf("draw frame:%dx%d, %p\n", f->mWidth, f->mHeight, f->mData);
            draw(f);
            eglSwapBuffers(mDisplay, mSurface);
            releaseFrame(f);
        }
    }

    printf("renderer thread exit...\n");
}

