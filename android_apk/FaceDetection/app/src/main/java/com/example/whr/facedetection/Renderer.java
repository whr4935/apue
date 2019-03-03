
package com.example.whr.facedetection;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.Surface;

public class Renderer {
    private static final String TAG = "JRenderer";

    private long mNativeObj;
    private SurfaceTexture mSurfaceTexture;
    private static native void native_init();
    private native void native_setup(Surface surface, Surface surfaceTexture);
    private native Surface native_getSurface();
    private native void native_setVideoSize(int width, int height);
    private native void native_setTexture2FaceDetection(boolean enable);
    private native void native_createFaceDetection();
    private native void native_sendFrame(byte[] data);
    private native void native_start();
    private native void native_stop();
    private native void native_finalize();

    static {
        System.loadLibrary("renderer");
        native_init();
    }

    public Renderer(Context context, Surface dispSurface) {
        RendererHelper.sContext = context;
        native_setup(dispSurface, new Surface(mSurfaceTexture));
    }

    public Surface getSurface() {
        Surface s = native_getSurface();
        return s;
    }

    public void setVideoSize(int width, int height) {
        native_setVideoSize(width, height);
    }

    public void setTexture2FaceDetection(boolean enable) {
        native_setTexture2FaceDetection(enable);
    }

    public void createFaceDetection() {
        native_createFaceDetection();
    }

    public void sendFrame(byte[] data) {
        native_sendFrame(data);
    }

    public void start() {
        native_start();
    }

    public void stop() {
        native_stop();
    }

    @Override
    protected void finalize() {
        native_finalize();
    }
}

