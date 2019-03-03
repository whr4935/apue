package com.example.whr.facedetection;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import java.lang.reflect.Method;
import java.util.List;

public class MainActivity extends AppCompatActivity implements Camera.PreviewCallback {

    private String TAG = "FaceDetection-hat";

    private SurfaceView mSurfaceView;
    private Surface mSurface;
    private Surface mRendererSurface;
    private Renderer mRenderer;
    private Camera mCamera;
    private byte[] mBuffer;

    private boolean mCameraFrameReady = false;
    private byte[] mPreviewData;
    private Thread mThread;
    private boolean mStopThread = false;

    static final int PREVIEW_WIDTH = 1280;
    static final int PREVIEW_HEIGHT = 720;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSurfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        SurfaceHolder holder = mSurfaceView.getHolder();
        holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                mSurface = holder.getSurface();
                mRenderer = new Renderer(MainActivity.this, mSurface);
                mRendererSurface = mRenderer.getSurface();

                if (mCamera != null) {
                    Class<Camera> c = Camera.class;
                    Method method = null;
                    try {
                        method = c.getMethod("setPreviewSurface", Surface.class);
                        method.invoke(mCamera, mRendererSurface);
                    } catch (Exception e) {
                        Log.e(TAG, "call setPreviewSurface failed:", e);
                    }
                }

                Camera.Parameters parameters = mCamera.getParameters();
                Camera.Size s = mCamera.new Size(PREVIEW_WIDTH, PREVIEW_HEIGHT);
                parameters.setPreviewSize(s.width, s.height);
                parameters.setPreviewFrameRate(25);
                mCamera.setParameters(parameters);

                parameters = mCamera.getParameters();
                int frameWidth = parameters.getPreviewSize().width;
                int frameHeight = parameters.getPreviewSize().height;
                int size = frameWidth * frameHeight;
                size  = size * ImageFormat.getBitsPerPixel(parameters.getPreviewFormat()) / 8;
                mBuffer = new byte[size];
                mCamera.addCallbackBuffer(mBuffer);
                mCamera.setPreviewCallbackWithBuffer(MainActivity.this);

                mRenderer.setVideoSize(s.width, s.height);
                mRenderer.setTexture2FaceDetection(false);
                mRenderer.createFaceDetection();
                mRenderer.start();

                try {
                    mCamera.startPreview();
                } catch (Exception e) {
                    Log.e(TAG, "Could not start preview", e);
                    mCamera.release();
                    mCamera = null;
                }
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                mRenderer.stop();
                if (mCamera != null)
                    mCamera.stopPreview();
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();

        mCamera = mCamera.open(Camera.CameraInfo.CAMERA_FACING_FRONT);

        mThread = new Thread(new CameraWorker());
        mThread.start();
    }

    @Override
    protected void onPause() {
        super.onPause();

        try {
            mStopThread = true;
            Log.d(TAG, "Notify thread");
            synchronized (this) {
                this.notify();
            }
            Log.d(TAG, "Wating for thread");
            if (mThread != null)
                mThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            mThread =  null;
        }

        if (mCamera != null) {
            mCamera.release();
            mCamera = null;
        }
    }

    private Camera.Size getBestSupportedSize(List<Camera.Size> sizes, int width, int height) {
        Camera.Size bestSize = sizes.get(0);
        int largestArea = bestSize.width * bestSize.height;
        for (Camera.Size s : sizes) {
            int area = s.width * s.height;
            if (area > largestArea) {
                bestSize = s;
                largestArea = area;
            }
        }
        return bestSize;
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        synchronized (this) {
            mPreviewData = data;
            mCameraFrameReady = true;
            this.notify();
        }

        if (mCamera != null)
            mCamera.addCallbackBuffer(mBuffer);
    }

private class CameraWorker implements Runnable {

    @Override
    public void run() {
        do {
            synchronized (MainActivity.this) {
                try {
                    while (!mCameraFrameReady && !mStopThread) {
                        MainActivity.this.wait();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if (mCameraFrameReady) {
                    mRenderer.sendFrame(mPreviewData);
                    mCameraFrameReady = false;
                }
            }
        } while (!mStopThread);
        Log.d(TAG, "Finish processing thread");
    }
}
}