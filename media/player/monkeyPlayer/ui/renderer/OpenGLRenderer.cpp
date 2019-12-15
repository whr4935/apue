#include "OpenGLRenderer.h"
#include <QPainter>
#include <QCoreApplication>

#include <QOpenGLShaderProgram>
#include <GL/gl.h>
#include <GL/glu.h>

#include <fstream>
#include <iostream>
#include <QDebug>
#include <QTimer>

#include <unistd.h>

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4


OpenGLRenderer::OpenGLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

    begin_test();

    mTimer = new QTimer;
    mTimer->setInterval(40);
    connect(mTimer, &QTimer::timeout, this, &OpenGLRenderer::test);


}

OpenGLRenderer::~OpenGLRenderer()
{
    end_test();
   // cleanup();
}

void OpenGLRenderer::initializeGL()
{
#if 1
    initializeOpenGLFunctions();

    GLint vertCompiled, fragCompiled, linked;    // 测试执行结果的
    GLint v, f;
    const char *vs,*fs;
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = loadShader("renderer/yuv420.vert");
    fs = loadShader("renderer/yuv420.frag");
    glShaderSource(v, 1, &vs,NULL);
    glShaderSource(f, 1, &fs,NULL);
    glCompileShader(v);
    glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);
    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glBindAttribLocation(p, ATTRIB_VERTEX, "position");
    glBindAttribLocation(p, ATTRIB_TEXTURE, "TexCoordIn");
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &linked);
    glUseProgram(p);
    glDeleteShader(v);
    glDeleteShader(f);
    delete [] vs;
    delete [] fs;

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
    textureUniformY = glGetUniformLocation(p, "tex_y");
    textureUniformU = glGetUniformLocation(p, "tex_u");
    textureUniformV = glGetUniformLocation(p, "tex_v");
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, coordVertices);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    glGenTextures(1, &id_y);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glGenTextures(1, &id_u);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &id_v);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    mTimer->start();

#endif
    return;
}

void OpenGLRenderer::paintGL()
{
#if 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w/2, h/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w/2, h/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[2]);

    glClearColor(0.0, 0.0, 0.0, 1.0);
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
#endif
}

void OpenGLRenderer::resizeGL(int w, int h)
{
    int video_width = this->w;
    int video_height = this->h;

    double video_ratio = (double)video_width / video_height;
    double win_ratio = (double)w / h;

    glViewport(0, 0, w/2, h/2);
}

void OpenGLRenderer::cleanup()
{
}

const char* OpenGLRenderer::loadShader(std::string filename)
{
    char* buffer = nullptr;
    std::fstream in(filename, std::ios_base::in);
    if (!in.is_open()) {
        std::cout << "open " << filename << " failed!" << std::endl;
        return buffer;
    }

    in.seekg(0, std::ios_base::end);
    int file_size = in.tellg();
    buffer = new char[file_size + 1]();
    in.seekg(0, std::ios_base::beg);
    in.read(buffer, file_size);
    if (in.gcount() != file_size) {
        std::cout << "read " << in.gcount() << " bytes!" << std::endl;
    }
    in.close();

    return buffer;
}

void OpenGLRenderer::setVideoSize(int width, int height)
{
    w = width;
    h = height;
}

int OpenGLRenderer::drawFrame(const char *pdata, size_t size)
{

    update();

    return 0;
}

/////////////////////////////////////////////////////
void OpenGLRenderer::begin_test()
{
    std::cout << "test start..." << std::endl;

    const char* filename = "1280_720.yuv";
    m_file.open(filename, std::ios_base::in|std::ios_base::binary);
    if (!m_file.is_open()) {
        std::cout << "open file:" << filename << " failed!" << std::endl;
        return;
    }

    int width = 1280;
    int height = 720;
    int buffer_size = width * height * 3 /2;
    m_pbuf = new char[buffer_size];

    setVideoSize(width, height);

    plane[0] = m_pbuf;
    plane[1] = plane[0] + w * h;
    plane[2] = plane[1] + w * h / 4;
}

void OpenGLRenderer::test()
{
int buffer_size = w * h * 3 /2;

        m_file.read(m_pbuf, buffer_size);
        if (m_file.gcount() != buffer_size) {
            std::cout << "read " << m_file.gcount() << " bytes" << std::endl;

            mTimer->stop();
            return;
        }

        drawFrame(m_pbuf, buffer_size);
}

void OpenGLRenderer::end_test()
{
    m_file.close();
    delete [] m_pbuf;
}
