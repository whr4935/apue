#ifndef KEYWORD

struct DumpStream;
static int write_dump_stream_pcm(struct DumpStream* stream, gchar* pdata, size_t size);
static int write_dump_stream_yuv(struct DumpStream* stream, gchar* pdata, size_t size);
static int write_dump_stream_mp3(struct DumpStream* stream, gchar* pdata, size_t size);
static int write_dump_stream_aac(struct DumpStream* stream, gchar* pdata, size_t size);
static int write_dump_stream_h264(struct DumpStream* stream, gchar* pdata, size_t size);

#define __MAKE_KEYWORD_ENUM__
#define KEYWORD(type, write_func) DUMP_STREAM_TYPE_##type,
enum DumpStreamType {
    DUMP_STREAM_TYPE_UNKNOWN,
#endif
    KEYWORD(pcm, dummy)
    KEYWORD(yuv, dummy)
    KEYWORD(mp3, dummy)
    KEYWORD(aac, dummy)
    KEYWORD(h264, dummy)
#ifdef __MAKE_KEYWORD_ENUM__
    DUMP_STREAM_TYPE_COUNT,
};
#undef __MAKE_KEYWORD_ENUM__
#undef KEYWORD
#endif
