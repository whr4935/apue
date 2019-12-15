#include "player.h"
#include "Audio.h"
#include "Video.h"

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "sdl.lib")


//////////////////////////////////////////////////////////////////////////
 VideoState *global_video_state;
 SDL_Surface *screen;
 SDL_mutex *screen_mutex;

 //////////////////////////////////////////////////////////////////////////
 int decode_thread(void *arg);
 int stream_component_open(VideoState *is, int stream_index);

 //////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	SDL_Event event;
	VideoState *is;
	int ret;

	if (argc != 2) {
		printf("Usage: ScreenCap <filepath>\n");
		exit(1);
	}
	
	is = (VideoState*)av_mallocz(sizeof(VideoState));

	av_register_all();
	ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (ret < 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		exit(1);
	}

	screen = SDL_SetVideoMode(640, 480, 0, 0);
	screen_mutex = SDL_CreateMutex();

	strncpy(is->filename, argv[1], sizeof(is->filename));
	is->pictq_mutex = SDL_CreateMutex();
	is->pictq_cond = SDL_CreateCond();

	schedule_refresh(is, 40);

	is->parse_tid = SDL_CreateThread(decode_thread, is);
	if (!is->parse_tid) {
		av_free(is);
	}

	for (;;) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case FF_QUIT_EVENT:
		case SDL_QUIT:
            printf("sdl quit!\n");
			is->quit = 1;
			SDL_Quit();
			return 0;
			break;

		case FF_REFRESH_EVENT:
			video_refresh_timer(event.user.data1);
			break;

		default:
			break;
		}
	}

	return 0;
}

int decode_thread(void *arg)
{
	VideoState *is = (VideoState*)arg;
	AVFormatContext *pFormatCtx = NULL;
	AVPacket pkt1, *packet = &pkt1;
    AVPacket dummy;

	int video_index = -1;
	int audio_index = -1;
	int i;

	is->videoStream = -1;
	is->audioStream = -1;

	global_video_state = is;

	if (avformat_open_input(&pFormatCtx, is->filename, NULL, NULL) != 0)
		return -1;

	is->pFormatCtx = pFormatCtx;

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		return -1;

	av_dump_format(pFormatCtx, 0, is->filename, 0);

	for (i = 0; i < pFormatCtx->nb_streams; ++i) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO &&
			video_index < 0) {
				video_index = i;
		}

		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
			audio_index < 0) {
				audio_index = i;
		}
	}

	if (audio_index >= 0) {
		stream_component_open(is, audio_index);
	}

	if (video_index >= 0) {
		stream_component_open(is, video_index);
	}

	if (is->videoStream < 0 || is->audioStream < 0) {
		printf("%s: could not open codecs\n", is->filename);
		goto fail;
	}

    int ret;
	for(;;) {
        if (is->quit) {
            AbortPakcetQueue(&is->videoq);
            AbortPakcetQueue(&is->audioq);
			break;
        }

		if (is->audioq.size > MAX_AUDIOQ_SIZE || 
			is->videoq.size > MAX_VIDEOQ_SIZE) {
				SDL_Delay(10);
				continue;
		}

		if ((ret = av_read_frame(is->pFormatCtx, packet)) < 0) {
            //printf("av_read_frame < 0!\n");
            //is->quit = true;
            if (ret == (int)AVERROR_EOF) {
                //printf("EOF\n");
                //goto fail;
            }

			if (is->pFormatCtx->pb->error == 0) {
                //printf("av_read_frame continue!\n");
				SDL_Delay(100);
                continue;
                //break;
			} else {
				break;
			}
		}

		if (packet->stream_index == is->videoStream) {
			PutPacketQueue(&is->videoq, packet);
		} else if (packet->stream_index == is->audioStream) {
			PutPacketQueue(&is->audioq, packet);
		} else {
			av_packet_unref(packet);
		}
	}

	while(!is->quit)
		SDL_Delay(100);

fail:
	if (1) {
		SDL_Event event;

		event.type = FF_QUIT_EVENT;
		event.user.data1 = is;
		SDL_PushEvent(&event);
	}

	return 0;
}

int stream_component_open(VideoState *is, int stream_index)
{
	AVFormatContext *pFormatCtx = is->pFormatCtx;
	AVCodecContext *codecCtx = NULL;
	AVCodec *codec = NULL;
	SDL_AudioSpec wanted_spec, spec;
    int64_t wanted_channel_layout = 0;
    int wanted_nb_channels;

	if (stream_index < 0 || stream_index >= pFormatCtx->nb_streams) {
		return -1;
	}

	codec = avcodec_find_decoder(pFormatCtx->streams[stream_index]->codec->codec_id);
	if (!codec) {
		printf("Unsupported codec!\n");
		return -1;
	}

	codecCtx = avcodec_alloc_context3(codec);
	if (avcodec_copy_context(codecCtx, pFormatCtx->streams[stream_index]->codec) != 0) {
		printf("Couldn't copy codec context\n");
		return -1;
	}

	if (codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
        wanted_nb_channels = codecCtx->channels;
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
		wanted_spec.freq = codecCtx->sample_rate;
		wanted_spec.format = AUDIO_S16SYS;
		wanted_spec.channels = codecCtx->channels;
		wanted_spec.silence = 0;
		wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
		wanted_spec.callback = audio_callback;
		wanted_spec.userdata = is;

		if (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
			printf("open audio error!\n");
			return -1;
		}
	}

	if (avcodec_open2(codecCtx, codec, NULL) != 0) {
		printf("Unsupported codec!\n");
		return -1;
	}

	switch (codecCtx->codec_type) {
	case AVMEDIA_TYPE_AUDIO:
		is->audioStream = stream_index;
		is->audio_st = pFormatCtx->streams[stream_index];
		is->audio_ctx = codecCtx;
		is->audio_buf_size = 0;
		is->audio_buf_index = 0;
        is->audio_src_fmt = is->audio_tgt_fmt = AV_SAMPLE_FMT_S16;
        is->audio_src_freq = is->audio_tgt_freq = spec.freq;
        is->audio_src_channel_layout = is->audio_tgt_channel_layout = wanted_channel_layout;
        is->audio_src_channels = is->audio_tgt_channels = spec.channels;
		memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
		InitPacketQueue(&is->audioq);
		SDL_PauseAudio(0);
		break;

	case AVMEDIA_TYPE_VIDEO:
		is->videoStream = stream_index;
		is->video_st = pFormatCtx->streams[stream_index];
		is->video_ctx = codecCtx;

		is->frame_timer = (double)av_gettime() / 1000000.0;
		is->frame_last_delay = 40e-3;

		InitPacketQueue(&is->videoq);
		is->video_tid = SDL_CreateThread(video_thread, is);
		is->sws_ctx = sws_getContext(is->video_ctx->width, is->video_ctx->height, \
			is->video_ctx->pix_fmt, is->video_ctx->width, \
			is->video_ctx->height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, \
			NULL, NULL, NULL);
		break;

	default:
		break;
	}

	return 0;
}


