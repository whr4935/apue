#include "renderer/Renderer.h"
#include "renderer/OpenGLRenderer.h"

#include <includes.h>
#include <MessageQueue.h>


int main(int argc, char *argv[])
{
#if 1
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    OpenGLRenderer w;
    w.show();

    return a.exec();

#else

    MessageQueue q;
    q.runAsync();
    sleep(10);
    std::cout << "ready to stop MessageQueue..." << std::endl;
    q.stop();

    return 0;

#endif
}
