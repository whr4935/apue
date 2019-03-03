package com.example.whr.facedetection;

import android.graphics.SurfaceTexture;

/**
 * Created by whr on 18-5-18.
 */

public class JSurfaceTexture extends SurfaceTexture implements SurfaceTexture.OnFrameAvailableListener {
    private static final String TAG = "JSurfaceTexture";
    private long mNativeSurfaceTexture;         //accessed by native methods

    private static native void native_init();

    private native void native_onFrameAvailable(long naitveSurface);

    private native void native_finalize();

    static {
        System.loadLibrary("jsurfaceTexture");
        native_init();
    }

    private JSurfaceTexture(int texName) {
        super(texName);
        super.setOnFrameAvailableListener(this);
    }

    public void updateTexImage() {
        super.updateTexImage();
    }

    public void getTransformMatrix(float[] mtx) {
        super.getTransformMatrix(mtx);
    }

    public long getTimestamp() {
        return super.getTimestamp();
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        native_onFrameAvailable(mNativeSurfaceTexture);
    }

    @Override
    protected void finalize() {
        native_finalize();
    }
}
