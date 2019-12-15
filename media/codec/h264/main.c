#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>

#define USE_FFMPEG_EXTRACTOR 1

#define INITIAL_BUF_SIZE (1<<16)

//////////////////////////////////////////////////
struct Source {
    FILE *fp;
    int eos;

    char *buf;
    int size;

    int offset;
    int length;
};

struct Decoder {
    AVCodec *codec;
    AVCodecContext *codec_context;
    int (*read_packet)(void *context, AVPacket *pkt);
};

struct Renderer {
    SDL_Surface *screen;
    SDL_Overlay *bmp;
};


//////////////////////////////////////////////////
int find_NAL_start(char *buf, int size)
{
    int i;

    for (i = 0; i+3 < size; ++i) {
        if (!memcmp(buf + i, "\x00\x00\x00\x01", 4))
            return i;
    }

    return -1;
}

int get_packet(struct Source *source, AVPacket *pkt)
{
    if (source->size == 0) {
        source->size = INITIAL_BUF_SIZE;
        source->buf = calloc(source->size, 1);
    }

    av_init_packet(pkt);

    int start_pos = -1;
    int end_pos = -1;
    for (;;) {
        int available_size = source->size - source->offset - source->length;
        int read_size;
        if (!source->eos && available_size > 0) {
            if ((read_size = fread( source->buf + source->offset + source->length, 1, available_size, source->fp)) < 0) {
                printf("fread error!\n");
                return -1;
            } else if (read_size == 0) {
                source->eos = 1;
            } else {
                source->length += read_size;
            }
        }

        if (source->eos && source->length == 0)
            return -1;

        int pos;
        if ((pos = find_NAL_start(source->buf + source->offset, source->length)) < 0) {
            if (start_pos < 0) {
                source->offset += source->length;
                if (source->offset >= source->size)
                    source->offset = 0;
                source->length -= source->length;
                continue;
            } else if (source->eos) {
                end_pos = source->offset + source->length;
                break;
            } else {
                if (source->offset < INITIAL_BUF_SIZE) {
                    int new_size = source->size << 1;
                    char *new_buf = calloc(new_size, 1);
                    printf("new buf size:%#x\n", new_size);
                    memcpy(new_buf, source->buf + source->offset, source->length);
                    source->buf = new_buf;
                    source->size = new_size;
                    source->offset = 0;
                    continue;
                } else {
                    memmove(source->buf, source->buf + source->offset, source->length);
                    source->offset = 0;
                    continue;
                }
            }
        } else {
            if (start_pos < 0) {
                start_pos = pos + 4;
                source->offset += start_pos;
                source->length -= start_pos;
            } else {
                end_pos = pos + source->offset;
                break;
            }
        }
    }

    int data_len = end_pos - source->offset;
    pkt->size = data_len + 4;
    pkt->data = av_mallocz(pkt->size);
    memcpy(pkt->data, "\x00\x00\x00\x01", 4);
    memcpy(pkt->data + 4, source->buf + source->offset, data_len);
    pkt->stream_index = 0;
    pkt->flags = 0;
    if ((pkt->data[4] & 0x1F) == 5)
        pkt->flags |= AV_PKT_FLAG_KEY;

    source->length -= data_len;
    source->offset += data_len;


    return 0;
}

int my_get_packet(struct Source *source, AVPacket *pkt)
{
    int ret;
    struct AVPacket packet_sps, packet_pps;
    int type;

    av_init_packet(&packet_sps);
    av_init_packet(&packet_pps);

    do {
        ret = get_packet(source, pkt);
        if (ret < 0)
            return -1;

        type = pkt->data[4] & 0x1F;
        if (type == 7) {
            packet_sps.data = pkt->data;
            packet_sps.size = pkt->size;
        } else if (type == 8) {
            packet_pps.data = pkt->data;
            packet_pps.size = pkt->size;
        }
    } while (type==7 || type==8);

    if (packet_sps.data || packet_pps.data) {
        char *buf = av_mallocz(packet_sps.size + packet_pps.size + pkt->size);
        memcpy(buf, packet_sps.data, packet_sps.size);
        memcpy(buf + packet_sps.size, packet_pps.data, packet_pps.size);
        memcpy(buf + packet_sps.size + packet_pps.size, pkt->data, pkt->size);
        av_free(pkt->data);
        pkt->data = buf;
        pkt->size += packet_sps.size + packet_pps.size;
        av_packet_unref(&packet_sps);
        av_packet_unref(&packet_pps);
    }

    return 0;
}

//////////////////////////////////////////////////
struct Source source;
struct Decoder decoder;
struct Renderer renderer;

AVFormatContext *context;
struct SwsContext *img_convert_ctx;

void *opaque;
int packet_pos;

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int ret;
    int video_stream_index = 0;

    if (argc != 2) {
        printf("Usage:%s filename\n", argv[0]);
        exit(1);
    }

    av_register_all();

    context = avformat_alloc_context();
    if (context == NULL) {
        printf("context is NULL\n");
        exit(1);
    }

    ret = avformat_open_input(&context, argv[1], NULL, NULL);
    if (ret < 0) {
        printf("oepn input failed!\n");
        exit(1);
    }

    if (avformat_find_stream_info(context, NULL)) {
        printf("find stream info failed!\n");
        return -1;
    }

    av_dump_format(context, 0, argv[1], 0);

    int i = 0;
    for (i = 0; i < context->nb_streams; ++i) {
        if (context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
        }
    }

    /* init decoder */
    decoder.codec = avcodec_find_decoder(context->streams[video_stream_index]->codec->codec_id);
    if (decoder.codec == NULL) {
        printf("codec is NULL!\n");
        exit(1);
    }

    decoder.codec_context = avcodec_alloc_context3(decoder.codec);
    if (avcodec_copy_context(decoder.codec_context, context->streams[video_stream_index]->codec) != 0) {
        printf("copy codec context failed!\n");
        exit(1);
    }

#if USE_FFMPEG_EXTRACTOR

    opaque = context;
    decoder.read_packet = (int (*)(void*, AVPacket*))av_read_frame;

#else
    source.fp = fopen(argv[1], "r+");
    if (source.fp == NULL) {
        printf("fopen file failed!\n");
        return -1;
    }

    opaque = &source;
    decoder.read_packet = my_get_packet;
#endif

    if (avcodec_open2(decoder.codec_context, decoder.codec, NULL) < 0) {
        printf("open codec failed!\n");
        exit(1);
    }

    ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret < 0) {
        SDL_Error(ret);
        exit(1);
    }
    atexit(SDL_Quit);

    renderer.screen = SDL_SetVideoMode(640, 480, 0, 0);
    if (renderer.screen == NULL) {
        printf("SDL_SetVideoMode failed!\n");
        exit(1);
    }

    SDL_Overlay *bmp = SDL_CreateYUVOverlay(decoder.codec_context->width, decoder.codec_context->height, SDL_YV12_OVERLAY, renderer.screen);
    if (bmp == NULL) {
        printf("bmp is NULL!\n");
        exit(1);
    }

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;

    SDL_WM_SetCaption("Simplest FFmpeg Player",NULL);

    AVFrame *pFrame=av_frame_alloc();
    AVPacket *av_packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    int got_picture;

    img_convert_ctx = sws_getContext(decoder.codec_context->width, decoder.codec_context->height, decoder.codec_context->pix_fmt,
                                     decoder.codec_context->width, decoder.codec_context->height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL);

    SDL_Event event;
    for (;;) {
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            goto quit;
        default:
            break;
        }

        if (decoder.read_packet(opaque, av_packet)>=0){
            packet_pos += av_packet->size;
            printf("pos:%#x\n", packet_pos);
            if(av_packet->stream_index==video_stream_index){

#if 1
                //Decode
                ret = avcodec_decode_video2(decoder.codec_context, pFrame, &got_picture,
                                            av_packet);
                if(ret < 0){
                    printf("Decode Error.\n");
                    return -1;
                }
                if(got_picture){
                    SDL_LockYUVOverlay(bmp);
                    AVPicture pict;
                    /*pict.data[0]=bmp->pixels[0];*/
                    /*pict.data[1]=bmp->pixels[2];*/
                    /*pict.data[2]=bmp->pixels[1];*/
                    /*pict.linesize[0]=bmp->pitches[0];*/
                    /*pict.linesize[1]=bmp->pitches[2];*/
                    /*pict.linesize[2]=bmp->pitches[1];*/
                    /*sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0,*/
                              /*decoder.codec_context->height, pict.data, pict.linesize);*/

                    SDL_UnlockYUVOverlay(bmp);
                    SDL_DisplayYUVOverlay(bmp, &rect);

                    SDL_Delay(30);
                }
#endif
            }
            av_packet_unref(av_packet);
        } else {
            goto quit;
        }
    }

quit:
    if (img_convert_ctx)
        sws_freeContext(img_convert_ctx);
    avcodec_close(decoder.codec_context);
    avformat_close_input(&context);
    avformat_free_context(context);
#if USE_FFMPEG_EXTRACTOR
    fclose(source.fp);
#endif

    SDL_Quit();
    printf("player exit!\n");
    exit(0);
}
