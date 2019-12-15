#ifndef OpenGLRenderer_H
#define OpenGLRenderer_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLPaintDevice>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <fstream>


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
class QTimer;

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLRenderer(QWidget *parent = 0);
    ~OpenGLRenderer();

    void setVideoSize(int width, int height);
    int drawFrame(const char* pdata, size_t size);

    void begin_test();
    void test();
    void end_test();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void cleanup();

private:
    const char* loadShader(std::string filename);

    GLuint p;                    // program id,着色器相关
    GLuint id_y, id_u, id_v; // 纹理id
    GLuint textureUniformY, textureUniformU,textureUniformV;
    int w, h;

    const char *plane[3];

    QTimer* mTimer;
    std::fstream m_file;
    char* m_pbuf;

};

#endif // OpenGLRenderer_H
