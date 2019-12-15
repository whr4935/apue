#include "Video.h"
#include "Audio.h"

Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque)
{
	SDL_Event event;
	
	event.type = FF_REFRESH_EVENT;
	event.user.data1 = opaque;
	SDL_PushEvent(&event);

	return 0;
}

void schedule_refresh(VideoState *is, int delay)
{
	SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

void video_display(VideoState *is)
{
	SDL_Rect rect;
	VideoPicture *vp;
	float aspect_ratio;
	int w, h, x, y;
	int i;

	vp = &is->pictq[is->pictq_rindex];
	if (vp->bmp) {
		if (is->video_ctx->sample_aspect_ratio.num == 0) {
			aspect_ratio = 0;
		} else {
			aspect_ratio = av_q2d(is->video_ctx->sample_aspect_ratio) * \
							is->video_ctx->width / is->video_ctx->height;
		}

		if (aspect_ratio <= 0.0) {
			aspect_ratio = (float)is->video_ctx->width / (float)is->video_ctx->height;
		}

		h = screen->h;
//		w = ((int)rint(h *aspect_ratio)) & ~3;
		w = ((int)(h *aspect_ratio)) & ~3;
		if (w > screen->w) {
			w = screen->w;
		}

		x = (screen->w - w) / 2;
		y = (screen->h - h) / 2;

		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_LockMutex(screen_mutex);
		SDL_DisplayYUVOverlay(vp->bmp, &rect);
		SDL_UnlockMutex(screen_mutex);
	}
}

void video_refresh_timer(void *userdata)
{
	VideoState *is = (VideoState*)userdata;
	VideoPicture *vp;
	double actual_delay, delay, sync_threshold, ref_clock, diff;

	if (is->video_st) {
		if (is->pictq_size == 0) {
			schedule_refresh(is, 1);
		} else {
			vp = &is->pictq[is->pictq_rindex];

			delay = vp->pts - is->frame_last_pts; /* the pts from last time */
			if (delay <= 0 || delay >= 1.0) {
				/* if incorrect delay, use previous one */
				delay = is->frame_last_delay;
			}
			/* save for next time */
			is->frame_last_delay = delay;
			is->frame_last_pts = vp->pts;

			/* update delay to sync to audio */
			ref_clock = get_audio_clock(is);
			diff = vp->pts - ref_clock;

			/* Skip or repeat the frame. Take delay into account
			FFPlay still doesn't "know if this is the best guess." */
			sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
			if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
				if (diff <= -sync_threshold) {
					delay = 0;
				} else if (diff >= sync_threshold) {
					delay = 2 * delay;
				}
			}
			is->frame_timer += delay;
			/* computer the REAL delay */
			actual_delay = is->frame_timer - (av_gettime() / 1000000.0);
			if (actual_delay < 0.010) {
				/* Really it should skip the picture instead */
				actual_delay = 0.010;
			}
			schedule_refresh(is, (int)(actual_delay * 1000 + 0.5));

			/* show the picture! */
			video_display(is);

			/* update queue for next picture! */
			if (++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) {
				is->pictq_rindex = 0;
			}
			SDL_LockMutex(is->pictq_mutex);
			is->pictq_size--;
			SDL_CondSignal(is->pictq_cond);
			SDL_UnlockMutex(is->pictq_mutex);
		}
	} else {
		schedule_refresh(is, 100);
	}
}

void alloc_picture(void *userdata)
{
	VideoState *is = (VideoState*)userdata;
	VideoPicture *vp;

	vp = &is->pictq[is->pictq_rindex];
	if (vp->bmp) {
		SDL_FreeYUVOverlay(vp->bmp);
	}

	SDL_LockMutex(screen_mutex);
	vp->bmp = SDL_CreateYUVOverlay(is->video_ctx->width,
									is->video_ctx->height,
									SDL_YV12_OVERLAY,
									screen);
	SDL_UnlockMutex(screen_mutex);

	vp->width = is->video_ctx->width;
	vp->height = is->video_ctx->height;
	vp->allocated = 1;
}

int queue_picture(VideoState *is, AVFrame *pFrame, double pts)
{
	VideoPicture *vp;
	int dst_pix_fmt;
	AVFrame pict;

	SDL_LockMutex(is->pictq_mutex);
	while (is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE &&
			!is->quit) {
				SDL_CondWait(is->pictq_cond, is->pictq_mutex);
	}
	SDL_UnlockMutex(is->pictq_mutex);

	if (is->quit)
		return -1;

	vp = &is->pictq[is->pictq_rindex];

	if (!vp->bmp ||
		vp->width != is->video_ctx->width ||
		vp->height != is->video_ctx->height) {
			SDL_Event event;

			vp->allocated = 0;
			alloc_picture(is);
			if (is->quit) {
				return -1;
			}
	}

	if (vp->bmp) {
		SDL_LockYUVOverlay(vp->bmp);

		vp->pts = pts;
		dst_pix_fmt = AV_PIX_FMT_YUV420P;

        pict.data[0] = vp->bmp->pixels[0];
        pict.data[1] = vp->bmp->pixels[2];
        pict.data[2] = vp->bmp->pixels[1];

        pict.linesize[0] = vp->bmp->pitches[0];
        pict.linesize[1] = vp->bmp->pitches[2];
        pict.linesize[2] = vp->bmp->pitches[1];

        sws_scale(is->sws_ctx, (uint8_t const* *)pFrame->data, \
                    pFrame->linesize, 0, is->video_ctx->height, \
                    pict.data, pict.linesize);

		SDL_UnlockYUVOverlay(vp->bmp);

		if (++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) {
			is->pictq_windex = 0;
		}

		SDL_LockMutex(is->pictq_mutex);
		is->pictq_size++;
		SDL_UnlockMutex(is->pictq_mutex);
	}

	return 0;
}

double synchronize_video(VideoState *is, AVFrame *src_frame, double pts) {

	double frame_delay;

	if (pts != 0) {
		/* if we have pts, set video clock to it */
		is->video_clock = pts;
	} else {
		/* if we aren't given a pts, set it to the clock */
		pts = is->video_clock;
	}
	/* update the video clock */
	frame_delay = av_q2d(is->video_ctx->time_base);
	/* if we are repeating a frame, adjust clock accordingly */
	frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
	is->video_clock += frame_delay;
	return pts;
}

int video_thread(void *arg)
{
	VideoState *is = (VideoState*)arg;
	AVPacket pkt1, *packet = &pkt1;
	int frameFinished = 0;
	AVFrame *pFrame;
	double pts;	

	pFrame = av_frame_alloc();

	for(;;) {
		if (GetPacketQueue(&is->videoq, packet, 1) < 0) {
			break;
		}

		pts = 0;
		avcodec_decode_video2(is->video_ctx, pFrame, &frameFinished, packet);

		if ((pts = av_frame_get_best_effort_timestamp(pFrame)) == AV_NOPTS_VALUE)
			pts = 0;

		pts *= av_q2d(is->video_st->time_base);

		if (frameFinished) {
			if (queue_picture(is, pFrame, pts) < 0) {
				break;
			}
		}
		av_packet_unref(packet);
	}

	av_frame_free(&pFrame);

	return 0;
}
