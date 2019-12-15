#include <iostream>
#include <cstring>
#include <cstdarg>
#include <fstream>

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "../surface.h"

#define die(s,...) \
        ({ \
         printf("Error line %d: " s "\n", __LINE__, ##__VA_ARGS__); \
         exit(-1);})

std::string yuvFileName;
int w = 1280;
int h = 720;
std::fstream m_file;
char* m_pbuf;
bool quit;

void sigHandler(int sig)
{
    write(STDOUT_FILENO, "\nquit...\n", 9);
    quit = true;
}

int main(int argc, char *argv[])
{
    argc = 2;
    argv[1] = (char*)"mbox/renderer/1280_720.yuv";

    if (argc == 2) {
        std::cout << "yuv play mode." << std::endl;
        yuvFileName = argv[1];
    }

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sigHandler;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        die("sigaction");
    }

    m_file.open(yuvFileName, std::ios_base::in | std::ios_base::binary);
    if (!m_file.is_open()) {
        die("open yuv file");
    }

    Surface renderer(w, h);
    renderer.start();
    Surface::Frame *f = nullptr;

    while (!quit) {
        f = renderer.dequeueFrame();
        if (f == nullptr) {
            usleep(1000);
            continue;
        }
        m_file.read((char*)f->data(), f->size());
        if (m_file.gcount() != f->size()) {
            quit = true;
            break;
        }
        renderer.queueFrame(f);
    }
    printf("exit...\n");
    renderer.stop();
    printf("exited!\n");

    return 0;
}
