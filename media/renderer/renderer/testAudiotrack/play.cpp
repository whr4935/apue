#include "../audiotrack.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    //AudioTrack::test();

    std::string path = getenv("PWD");
    const char* filepath = nullptr;
    if (argc != 2) {
        path += "/mbox/renderer/a.pcm";
        printf("path:%s\n", path.c_str());
        if (access(path.c_str(), F_OK) < 0) {
            printf("Arguments: filepath!\n");
            return 0;
        }
        filepath = path.c_str();
    }

    FILE* fp = fopen(filepath, "r");
    if (fp == NULL) {
        printf("open %s failed:%s\n", argv[1], strerror(errno));
        return -1;
    }
    
    int size = 1024;
    char* buffer = new char[size];
    if (buffer == NULL) {
        printf("new buffer failed!\n");
        return -1;
    }

    int len;

    AudioTrack at;
    at.start();
    while (!feof(fp)) {
        len = fread(buffer, 1, size, fp);
        at.write((unsigned char*)buffer, len);
    }
    at.stop();

    delete[] buffer;
    fclose(fp);

    return 0;
}
