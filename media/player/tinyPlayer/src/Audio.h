#ifndef AUDIO_H
#define AUDIO_H

#include "player.h"

extern PacketQueue audioQueue;

void audio_callback(void *userdata, Uint8 *stream, int len);
double get_audio_clock(VideoState *is);

#endif