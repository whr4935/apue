#include "Audio.h"
#include <libswresample/swresample.h>

//////////////////////////////////////////////////////////////////////////
int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size, double*);

void audio_callback(void *userdata, Uint8 *stream, int len)
{
	VideoState *is = (VideoState*)userdata;
	int len1, audio_size;
	double pts;

	while (len > 0) {
		if (is->audio_buf_index >= is->audio_buf_size) {
			audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf), &pts);
			if (audio_size < 0) {
				is->audio_buf_size = 1024;
				memset(is->audio_buf, 0, is->audio_buf_size);
			} else {
				is->audio_buf_size = audio_size;
			}
			is->audio_buf_index = 0;
		}

		len1 = is->audio_buf_size - is->audio_buf_index;
		if (len1 > len)
			len1 = len;
		memcpy(stream, (uint8_t*)is->audio_buf + is->audio_buf_index, len1);
		len -= len1;
		stream += len1;
		is->audio_buf_index += len1;
	}
}

int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size, double *pts_ptr)
{
	int len1, len2;
	int decoded_data_size = 0;
	AVPacket *pkt = &is->audio_pkt;
	double pts;
	int n;
    int64_t dec_channel_layout;
    int wanted_nb_samples, resampled_data_size;

	for (;;) {
		while (is->audio_pkt_size > 0) {
			int got_frame = 0;

			//avcodec_get_frame_defaults(&is->audio_frame);
			len1 = avcodec_decode_audio4(is->audio_ctx, &is->audio_frame, &got_frame, pkt);
			if (len1 < 0) {
				is->audio_pkt_size = 0;
				printf("decode audio failed!\n");
				break;
			}

			is->audio_pkt_data += len1;
			is->audio_pkt_size -= len1;
            if (!got_frame) continue;

			decoded_data_size = 0;
				decoded_data_size = av_samples_get_buffer_size(NULL,
					is->audio_ctx->channels,
					is->audio_frame.nb_samples,
					is->audio_ctx->sample_fmt,
					1);
				//assert(data_size <= buf_size);
                dec_channel_layout = (is->audio_frame.channel_layout && is->audio_frame.channels==
                        av_get_channel_layout_nb_channels(is->audio_frame.channel_layout)) ?
                    is->audio_frame.channel_layout : av_get_default_channel_layout(is->audio_frame.channels);
                wanted_nb_samples = is->audio_frame.nb_samples;
                if (is->swr_ctx)
                    swr_free(&is->swr_ctx);
                is->swr_ctx = swr_alloc_set_opts(NULL, is->audio_tgt_channel_layout,
                        is->audio_tgt_fmt,
                        is->audio_tgt_freq,
                        dec_channel_layout,
                        (AVSampleFormat)is->audio_frame.format,
                        is->audio_frame.sample_rate, 0, NULL);
                if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
                    fprintf(stderr, "swr_init() failed!\n");
                    break;
                }
                is->audio_src_channel_layout = dec_channel_layout;
                is->audio_src_channels = is->audio_st->codec->channels;
                is->audio_src_freq = is->audio_st->codec->sample_rate;
                is->audio_src_fmt = is->audio_st->codec->sample_fmt;
                if (is->swr_ctx) {
                    const uint8_t **in = (const uint8_t**) is->audio_frame.extended_data;
                    uint8_t* out[] = {is->audio_buf2};
                    len2 = swr_convert(is->swr_ctx, out, sizeof(is->audio_buf2)/is->audio_tgt_channels/av_get_bytes_per_sample(is->audio_tgt_fmt),
                            in, is->audio_frame.nb_samples);
                    
                    if (len2 < 0) {
                        fprintf(stderr, "swr_convert() failed\n");
                        break;
                    }
                    if (len2 == sizeof(is->audio_buf2) / is->audio_tgt_channels / av_get_bytes_per_sample(is->audio_tgt_fmt)) {
                        fprintf(stderr, "warning: audio buffer is probably too small\n");
                        swr_init(is->swr_ctx);
                    }
                    resampled_data_size = len2 * is->audio_tgt_channels * av_get_bytes_per_sample(is->audio_tgt_fmt);
                    memcpy(audio_buf, is->audio_buf2, resampled_data_size);
                } else {
                    resampled_data_size = decoded_data_size;
                    memcpy(audio_buf, is->audio_frame.data[0], resampled_data_size);
                }

			//if (decoded_data_size <= 0) {
				//continue;
			//}

			pts = is->audio_clock;
			*pts_ptr = pts;
			n = 2 * is->audio_ctx->channels;
			is->audio_clock += (double)resampled_data_size /
				(double)(n * is->audio_ctx->sample_rate);

			return resampled_data_size;
		}

		if (pkt->data) {
			av_packet_unref(pkt);
		}

		if (is->quit)
			return -1;

		if (GetPacketQueue(&is->audioq, pkt, 1) < 0) {
			return -1;
		}

		is->audio_pkt_data = pkt->data;
		is->audio_pkt_size = pkt->size;

		if (pkt->pts != AV_NOPTS_VALUE) {
			is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
		}
	}
}

double get_audio_clock(VideoState *is)
{
	double pts;
	int hw_buf_size, bytes_per_sec, n;

	pts = is->audio_clock; /* maintained in the audio thread */
	hw_buf_size = is->audio_buf_size - is->audio_buf_index;
	bytes_per_sec = 0;
	n = is->audio_ctx->channels * 2;
	if (is->audio_st) {
		bytes_per_sec = is->audio_ctx->sample_rate * n;
	}
	if (bytes_per_sec) {
		pts -= (double)hw_buf_size / bytes_per_sec;
	}
	return pts;
}
