#ifndef		_PLAYER_H
#define		_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus
};
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#ifdef main
#undef main
#endif

#include "PacketQueue.h"

#define		VIDEO_PICTURE_QUEUE_SIZE		3

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE		(5 * 16 * 1024)
#define	MAX_VIDEOQ_SIZE		(5 * 256 * 1024)

#define	FF_REFRESH_EVENT	(SDL_USEREVENT)
#define	FF_QUIT_EVENT		(SDL_USEREVENT + 1)

#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0

//////////////////////////////////////////////////////////////////////////
typedef struct VideoPicture {
	SDL_Overlay *bmp;
	int width, height;
	int allocated;
	double pts;
}VideoPicture;

typedef struct VideoState {
	AVFormatContext *pFormatCtx;
	int				videoStream, audioStream;

	double			audio_clock;
	AVStream		*audio_st;
	AVCodecContext	*audio_ctx;
	PacketQueue		audioq;
	uint8_t			audio_buf[MAX_AUDIO_FRAME_SIZE * 3 / 2];
	uint8_t			audio_buf2[MAX_AUDIO_FRAME_SIZE * 3 / 2];
	unsigned int	audio_buf_size;
	unsigned int	audio_buf_index;
	AVFrame			audio_frame;
	AVPacket		audio_pkt;
	uint8_t			*audio_pkt_data;
	int				audio_pkt_size;
	int				audio_hw_buf_size;
    SwrContext      *swr_ctx;
    AVSampleFormat  audio_tgt_fmt;
    int             audio_tgt_freq;
    int             audio_tgt_channel_layout;
    int             audio_tgt_channels;
    AVSampleFormat  audio_src_fmt;
    int             audio_src_freq;
    int64_t         audio_src_channel_layout;
    int             audio_src_channels;


	double			frame_timer;
	double			frame_last_pts;
	double			frame_last_delay;
	double			video_clock;

	AVStream		*video_st;
	AVCodecContext	*video_ctx;
	PacketQueue		videoq;
	struct SwsContext *sws_ctx;

	VideoPicture	pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int				pictq_size, pictq_rindex, pictq_windex;
	SDL_mutex		*pictq_mutex;
	SDL_cond		*pictq_cond;

	SDL_Thread		*parse_tid;
	SDL_Thread		*video_tid;

	char			filename[256];
	int				quit;
}VideoState;

//////////////////////////////////////////////////////////////////////////
extern VideoState *global_video_state;



#endif
