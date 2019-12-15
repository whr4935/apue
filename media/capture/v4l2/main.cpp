/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.05
 */

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sstream>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <assert.h>
extern "C" {
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
}


#define die() \
do { \
    std::cout << "line:" << __LINE__ <<  "  error:" << errno << " " << strerror(errno) << std::endl; \
    exit(EXIT_FAILURE); \
} while (0)

typedef struct {
    void *start;
    size_t length;
} VideoBuffer;

std::string print_fourcc(unsigned int fourcc)
{
    std::stringstream ss;
    char*d = (char*)&fourcc;
    ss << d[0] << d[1] << d[2] << d[3];
    return ss.str();
}

int main(int argc, char *argv[])
{
    int cameraFd;
    cameraFd = open("/dev/video0", O_RDWR|O_NONBLOCK);
    if (cameraFd < 0) {
        std::cout << "open /dev/video0 failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    int dumpFd;
    dumpFd = open("record.yuv", O_RDWR|O_TRUNC|O_CREAT, 0666);
    if (dumpFd < 0) {
        die();
    }

    struct v4l2_capability capability;
    int ret = ioctl(cameraFd, VIDIOC_QUERYCAP, &capability);
    if (ret < 0) {
        die();
    }

    if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        std::cout << "not an video capture device!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
        std::cout << "does not support streaming i/o" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream ss;
    ss << "Driver name: " << capability.driver;
    std::cout << ss.str() << std::endl;

    int index = 0;
    ret = ioctl(cameraFd, VIDIOC_G_INPUT, &index);
    if (ret < 0) {
        die();
    }
    std::cout << "device index: " << index << std::endl;

    struct v4l2_input input;
    memset(&input, 0, sizeof(input));
    input.index = index;
    ret = ioctl(cameraFd, VIDIOC_ENUMINPUT, &input);
    if (ret < 0) die();
    std::cout << "dump v4l2_input:" << std::endl;
    std::cout << "  index:" << input.index << std::endl;
    std::cout << "  name:" << input.name << std::endl;
    std::cout << "  type:" << input.type << std::endl;
    std::cout << "  std:" << input.std << std::endl;
    std::cout << "  status:" << input.status << std::endl;
    std::cout << "  capabilities:" << input.capabilities << std::endl;

    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 640;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(cameraFd, VIDIOC_S_FMT, &fmt) < 0) {
        die();
    }

    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cameraFd, VIDIOC_G_FMT, &fmt) < 0) {
        die();
    }
    std::cout << "\nv4l2_format:" << std::endl;
    std::cout << "  width:" << fmt.fmt.pix.width << std::endl;
    std::cout << "  height:" << fmt.fmt.pix.height << std::endl;
    std::cout << "  pixel_format:" << print_fourcc(fmt.fmt.pix.pixelformat) << std::endl;
    std::cout << "  field:" << fmt.fmt.pix.field << std::endl;

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(cameraFd, VIDIOC_REQBUFS, &req) < 0) {
        if (EINVAL == errno) {
            std::cout << "does not surrpot memory mapping!\n" << std::endl;
            return -1;
        } else {
            die();
        }
    }

    VideoBuffer* buffers = (VideoBuffer*)calloc(req.count, sizeof(*buffers));
    struct v4l2_buffer buf;
    struct v4l2_buffer bufs[4];
    for (int numBufs = 0; numBufs < req.count; ++numBufs) {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = numBufs;
        if (ioctl(cameraFd, VIDIOC_QUERYBUF, &buf) == -1)
            return -1;
        bufs[numBufs] = buf;
        buffers[numBufs].length = buf.length;
        buffers[numBufs].start = mmap(NULL, buf.length,
                PROT_READ|PROT_WRITE, MAP_SHARED, cameraFd, buf.m.offset);
        if (buffers[numBufs].start == MAP_FAILED) {
            return -1;
        }
    }

    for (int i = 0; i < 4; ++i) {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.m.offset = bufs[i].m.offset;

        if (ioctl(cameraFd, VIDIOC_QBUF, &buf) < 0) {
            die();
        }
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cameraFd, VIDIOC_STREAMON, &type) < 0) {
        die();
    }
    std::cout << "begin... " << std::endl;

    struct SwsContext *swsContext = sws_getContext(640, 480,
            AV_PIX_FMT_YUYV422, 640, 480, AV_PIX_FMT_YUV420P,SWS_BILINEAR, NULL, NULL, NULL);
    AVFrame* pSrcFrame, *pDstFrame;
    pSrcFrame = av_frame_alloc();
    pSrcFrame->width = 640;
    pSrcFrame->height = 480;
    pSrcFrame->format = AV_PIX_FMT_YUYV422;
    av_frame_get_buffer(pSrcFrame, 0);
    std::cout << "linesize:" << pSrcFrame->linesize[0] << std::endl;

    pDstFrame = av_frame_alloc();
    pDstFrame->width = 640;
    pDstFrame->height = 480;
    pDstFrame->format = AV_PIX_FMT_YUV420P;
    av_frame_get_buffer(pDstFrame, 0);
    //pDstFrame->linesize[1] = 160;
    //pDstFrame->linesize[2] = 160;
    std::cout << "linesize:" << pDstFrame->linesize[0] << std::endl;
    std::cout << "linesize:" << pDstFrame->linesize[1] << std::endl;
    std::cout << "linesize:" << pDstFrame->linesize[2] << std::endl;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(cameraFd, &fds);
    int maxFds = cameraFd+1;
    struct timeval tv;
    int frame_cnt = 100;
    while (frame_cnt) {
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        ret = select(maxFds, &fds, nullptr, nullptr, &tv);
        if (ret < 0) {
            die();
        } else if (ret == 0) {

        } else {
            if (FD_ISSET(cameraFd, &fds)) {
                struct v4l2_buffer capture_buf;
                memset(&capture_buf, 0, sizeof(capture_buf));
                capture_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                capture_buf.memory = V4L2_MEMORY_MMAP;
                if (ioctl(cameraFd, VIDIOC_DQBUF, &capture_buf) < 0) {
                    std::cout << "error VIDIOC_DQBUF" << std::endl;
                    return -1;
                }
                assert(capture_buf.index<4);

                pSrcFrame->data[0] = (uint8_t*)buffers[capture_buf.index].start;
                ret = sws_scale(swsContext, pSrcFrame->data, pSrcFrame->linesize, 0, 0, pDstFrame->data, pDstFrame->linesize);
                //std::cout << "sws_scale ret " << ret << std::endl;
                //std::cout << pDstFrame->linesize[0] << std::endl;
                //std::cout << pDstFrame->linesize[1] << std::endl;
                //std::cout << pDstFrame->linesize[2] << std::endl;

                write(dumpFd, buffers[capture_buf.index].start, capture_buf.bytesused);

                //write(dumpFd, pDstFrame->data[0], pDstFrame->linesize[0]*480);
                //write(dumpFd, pDstFrame->data[1], pDstFrame->linesize[1]*480);
                //write(dumpFd, pDstFrame->data[2], pDstFrame->linesize[2]*480);

                --frame_cnt;

                if (ioctl(cameraFd, VIDIOC_QBUF, &capture_buf) < 0) {
                    die();
                }
            }
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cameraFd, VIDIOC_STREAMOFF, &type) < 0) {
        die();
    }

    close(dumpFd);

    return 0;
}
