#ifndef _RENDER_FRAME_AVAILABLE_LISTENER_H
#define _RENDER_FRAME_AVAILABLE_LISTENER_H

#include "renderer.h"

class Renderer::FrameAvailableListener {
public:
    FrameAvailableListener(Renderer *renderer)
        : mRenderer(renderer) {

        }

    ~FrameAvailableListener() {

    }

    void onFrameAvailable(NativeSurfaceTexture *st) {
        mRenderer->onFrameAvailable(st);
    }


private:
    Renderer *mRenderer;
};



#endif
