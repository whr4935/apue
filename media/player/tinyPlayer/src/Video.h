#ifndef		_VIDEO_H
#define		_VIDEO_H

#include "player.h"

extern  SDL_Surface *screen;
extern SDL_mutex *screen_mutex;


void schedule_refresh(VideoState *is, int delay);
void video_refresh_timer(void *userdata);
int video_thread(void *arg);

#endif