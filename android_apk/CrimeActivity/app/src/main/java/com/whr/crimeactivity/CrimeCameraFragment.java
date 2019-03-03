package com.whr.crimeactivity;


import android.app.Activity;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.io.IOException;
import java.util.List;

/**
 * Created by whr on 17-4-28.
 */

public class CrimeCameraFragment extends Fragment {
    private static final String TAG="CrimeCameraFragment";

    private Camera mCamera;
    private SurfaceView mSurfaceView;
    private MyThread mThread;

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        Log.d(TAG, "onAttach");
    }

    @Override
    public  void onCreate(Bundle savedInstaceState) {
        super.onCreate(savedInstaceState);
        Log.d(TAG, "onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup parent, Bundle savedInstanceState) {
        Log.d(TAG, "onCreateView");
        View v = inflater.inflate(R.layout.fragment_crime_camera, parent, false);

        Button takePictureButton = (Button)v.findViewById(R.id.crime_camera_takePictureButton);
        takePictureButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                getActivity().finish();
            }
        });

        mSurfaceView = (SurfaceView)v.findViewById(R.id.crime_camera_surfaceView);
        SurfaceHolder holder = mSurfaceView.getHolder();
//        holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        holder.addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                try {
                    if (mCamera != null) {
                        Log.d(TAG, "camerea set previewDisplay");
                        mCamera.setPreviewDisplay(holder);
                    }
                } catch (Exception e) {
                    Log.e(TAG, "Error setting up preview display", e);
                }

                if (mThread == null) {
                    mThread = new MyThread(holder);
                }
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                if (mCamera == null ) return;

                Camera.Parameters parameters = mCamera.getParameters();
                Camera.Size s = getBestSupportedSize(parameters.getSupportedPreviewSizes(), width, height);
                parameters.setPreviewSize(s.width, s.height);
                mCamera.setParameters(parameters);
                try {
                    Log.d(TAG, "camera start preview");
                    mCamera.startPreview();

                } catch (Exception e) {
                    Log.e(TAG, "Could not start preview", e);
                    mCamera.release();
                    mCamera = null;
                }

                if (mThread != null) {
                    mThread.start();
                }
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                if (mCamera != null)
                    mCamera.stopPreview();

                if (mThread != null)
                    mThread.isRun = false;
            }
        });



        return v;
    }

    @Override
    public  void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        Log.d(TAG, "onAcitivityCreated");
    }

    @Override
    public  void onStart() {
        super.onStart();
        Log.d(TAG, "onStart");
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");

        mCamera = Camera.open();

        Log.d(TAG, "camera opend");
    }

    @Override
    public void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();

        if (mCamera != null) {
            mCamera.release();
            mCamera = null;
        }
    }

    private Size getBestSupportedSize(List<Size> sizes, int width , int height) {
        Size bestSize = sizes.get(0);
        int largestArea = bestSize.width * bestSize.height;
        for (Size s : sizes) {
            int area = s.width * s.height;
            if (area > largestArea) {
                bestSize = s;
                largestArea = area;
            }
        }

        return bestSize;
    }

    class MyThread extends Thread {
        private SurfaceHolder mHolder;
        public boolean isRun;

        public MyThread(SurfaceHolder holder) {
            this.mHolder = holder;
            isRun = true;
        }

        @Override
        public void run() {
            int count = 0;
            while (isRun) {
                Canvas c = null;
                try {
                    synchronized (mHolder) {

                        c = mHolder.lockCanvas();
                        if (c != null)
                            Log.d(TAG, "lock canvas success!" );
                        else
                            Log.d(TAG, "lock canvas failed!");
                        c.drawColor(Color.BLUE);
                        Paint p = new Paint();
                        p.setColor(Color.RED);
                        Rect r = new Rect(100, 50, 300, 250);
                        c.drawRect(r, p);
                        Thread.sleep(1000);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    if (c != null) {
                        mHolder.unlockCanvasAndPost(c);
                    }
                }
            }
        }
    }
}
