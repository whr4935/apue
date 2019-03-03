//
// Created by whr on 18-5-18.
//

#ifndef FACEDETECTION_NATIVESURFACETEXTURE_H
#define FACEDETECTION_NATIVESURFACETEXTURE_H

#include <cstdint>

class NativeSurfaceTexture {
public:
    struct OnFrameAvailableListener {
        virtual ~OnFrameAvailableListener() {};
        virtual void onFrameAvailable(NativeSurfaceTexture* st) = 0;
    };

public:
    explicit NativeSurfaceTexture(int texName);
    ~NativeSurfaceTexture();
    void setFrameAvailableListener(OnFrameAvailableListener *listener);
    void onFrameAvailable();

    int updateTexImage() const;
    void getTransformMatrix(float mtx[16]);
    int64_t getTimestamp();

private:
    void* mJSurfaceTexture;
    OnFrameAvailableListener *mFrameAvailableListener;
};

#endif //FACEDETECTION_NATIVESURFACETEXTURE_H
