#include <librtmp/rtmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RTMP *rtmp;

int main(int argc, char *argv[])
{
    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);    

    char url[] = "rtmp://live.hkstv.hk.lxdns.com/live/hks";
    if (RTMP_SetupURL(rtmp, url) == FALSE) {
        printf("seturl failed!\n");
        RTMP_Free(rtmp);
        return -1;
    }

    //RTMP_EnableWrite(rtmp);

    if (RTMP_Connect(rtmp, NULL) == FALSE) {
        printf("connect failed!\n");
        RTMP_Free(rtmp);
        return -1;
    }

    if (RTMP_ConnectStream(rtmp, 0) == FALSE) {
        printf("connect stream failed!\n");
        RTMP_Free(rtmp);
        return -1;
    }

    char* buf = new char[65536];
    char filename[] = "rtmp.file";
    FILE* fp = nullptr;
    fp = fopen(filename, "w");
    if (fp == nullptr) {
        printf("open %s failed:%s\n", filename, strerror(errno));
        exit(-1);
    }
    int ret;
    int writeLen = 0;
    for (;;) {
        ret = RTMP_Read(rtmp, buf, 65536);
        if (ret <= 0) {
            printf("rtmp read:%d\n", ret);
            break;
        }

        writeLen = fwrite(buf, 1, ret, fp);
        if (writeLen != ret) {
            printf("frwite error:%s\n", strerror(errno));
            break;
        }

        //printf("rtmp read size:%d\n", ret);
    }
    delete[] buf;

    fclose(fp);


    RTMP_Close(rtmp);
    RTMP_Free(rtmp);

    return 0;
}
