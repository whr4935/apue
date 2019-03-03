package com.example.appsprit.videoview;

import android.content.ContentResolver;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.VideoView;

import java.io.FileDescriptor;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    static String TAG = "WHR";
    private SurfaceView mSurfaceView = null;
    private MediaPlayer mPlayer = null;
    private AssetFileDescriptor mFd = null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSurfaceView = findViewById(R.id.surfaceView);

        mPlayer = new MediaPlayer();
        mPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                Log.d(TAG, "onPrepared:");
                mPlayer.start();
            }
        });

        mPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                Log.d(TAG, "error " + what);
                return false;
            }
        });




//        mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
//            @Override
//            public void surfaceCreated(SurfaceHolder holder) {
//                Surface surface = holder.getSurface();
//                mPlayer.setSurface(surface);
//                mPlayer.prepareAsync();
//            }
//
//            @Override
//            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
//
//            }
//
//            @Override
//            public void surfaceDestroyed(SurfaceHolder holder) {
//
//            }
//        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        String packagetName = getPackageName();
        Uri uri = Uri.parse("android.resource://" + getPackageName() + "/" + R.raw.clock);
        Log.i(TAG, uri.toString());

        try {
            ContentResolver resolver = getContentResolver();
            mFd = resolver.openAssetFileDescriptor(uri, "r");
            if (mFd == null) {
                Log.i(TAG, "mFd is NULL");
                return;
            }
        }  catch (IOException ex) {
            Log.d(TAG, "openAssetFileDescriptor " + ex.getMessage());
        }

        if (mFd == null)
            return;

        try {
            mPlayer.setDataSource(mFd);
        } catch (IOException e) {
            e.printStackTrace();
        }

        mPlayer.setSurface(mSurfaceView.getHolder().getSurface());
        mPlayer.prepareAsync();
    }


}
