package com.whr.hellomoon;

import android.content.Context;
import android.media.MediaPlayer;
import android.util.Log;

/**
 * Created by whr on 17-4-29.
 */
public class AudioPlayer {
    static final String TAG = "audioPlayer";

    private MediaPlayer mPlayer;

    public void stop() {
        if (mPlayer != null) {
            mPlayer.release();
            mPlayer = null;
        }
    }

    public void play(Context c) {
        stop();

        mPlayer = MediaPlayer.create(c, R.raw.one_small_step);

        mPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                Log.d(TAG, "received onCompletion");
                stop();
            }
        });

        mPlayer.start();
        Log.d(TAG,"play started");
    }
}
