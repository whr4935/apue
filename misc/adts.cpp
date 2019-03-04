#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static int inline readAacFrameSize(const uint8_t* p)
{
    int aacFrameSize = (p[3] & 0x3) << 11 | (p[4]) << 3 | (p[5] >> 5);

    return aacFrameSize;
}

static void inline setAacFrameSize(uint8_t* p, int frameSize)
{
    p[3] &= ~0x03;
    p[3] |= frameSize >> 11;

    p[4] = frameSize >> 3 & 0xFF;

    p[5] &= ~0xE0;
    p[5] |= (frameSize & 0x07) << 5;
}

size_t testFilter(const uint8_t* in, size_t inAllocLen, uint8_t* outPtr)
{
    size_t offset = 0;
    const uint8_t hack[] = {0xC0,  0x00, 0x00, 0x00, 0x01, 0xCE, 0x8C, 0x4D, 0x9D, 0x10, 0x8E, 0x25, 0xE9};

    uint8_t* baseOutPtr = outPtr;

    while (offset < inAllocLen) {
        if (in[offset] != 0xFF || (in[offset] & 0xF0) != 0xF0) {
            ++offset;
            continue;
        }

        const uint8_t* p = in + offset;
        const uint8_t* pBegin = in + offset;
        int frameSize = readAacFrameSize(pBegin);

        if (offset + frameSize > inAllocLen) {
            printf("error size! offset:%d, frameSize:%d, inAllocLen:%d", offset, frameSize, inAllocLen);
            break;
        }

        const uint8_t* pEnd = pBegin + frameSize;
        bool found = false;

        while (p < pEnd) {
            if (*p == 0xC0) {
                if (!memcmp(hack, p, 14)) {
                    found = true;

                    memcpy(outPtr, pBegin, p - pBegin);
                    outPtr += p - pBegin;
                    p += 14;
                    pBegin = p;
                }
            } else {
                ++p;
            }
        }

        if (!found) {
            memcpy(outPtr, pBegin, frameSize);
            outPtr += frameSize;
        } else if (pBegin < pEnd) {
            memcpy(outPtr, pBegin, pEnd - pBegin);
            outPtr += pEnd - pBegin;
        }

        offset += frameSize;
    }

    return outPtr - baseOutPtr;
}

void parseAac(const uint8_t* in, size_t size)
{
    const uint8_t* pEnd = in + size;

    size_t offset = 0;
    while (offset < size) {
        if (in[offset]!=0xFF || (0xF0&in[offset+1])!=0xF0) {
            printf("don't equal start code, offset:%#x, %#x %#x\n", offset, in[offset], in[offset+1]);
            ++offset;
            continue;
        }
        

        const uint8_t* pBegin = in + offset;
        size_t frameSize = readAacFrameSize(pBegin);
        printf("frame offset:%#x size:%d\n", offset, frameSize);

        offset += frameSize;
    }

    printf("finished!\n");
}


int main(int argc, char* argv[])
{
    uint8_t adts[] = {0xff, 0xf1, 0x4c, 0x80, 0x39, 0x3f, 0xfc};

    //printf("frameSize:%d\n", readAacFrameSize(adts));

    //size_t s1 = readAacFrameSize(adts);
    //s1 -= 14;

    //setAacFrameSize(adts, s1);
    //printf("frameSize:%d\n", readAacFrameSize(adts));

    FILE* fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("fp1 NULL");
        return 0;
    }

    struct stat st;
    size_t fileSzie = 0;
    int ret = stat(argv[1], &st);
    if (ret == 0) {
        fileSzie = st.st_size;
    } else {
        printf("stat error:%d", errno);
        return 0;
    }

    uint8_t* readBuf = new uint8_t[fileSzie];
    ret = fread(readBuf, 1, fileSzie, fp1);
    if (ret != fileSzie) {
        printf("read failed:%d, errno:%d\n", ret, errno);
        return 0;
    }

    //test one
    parseAac(readBuf, fileSzie);
    return 0;


    //////////////////////////////////

    FILE* fp2 = fopen("out.aac", "w");
    if (fp2 == NULL) {
        printf("fp2 NULL");
        return 0;
    }

    uint8_t* outbuf = new uint8_t[fileSzie];
    size_t filterRet = testFilter(readBuf, fileSzie, outbuf);
    printf("filterRet:%zu\n", filterRet);

    fwrite(outbuf, 1, filterRet, fp2);

    fclose(fp1);
    fclose(fp2);

    return 0;
}
