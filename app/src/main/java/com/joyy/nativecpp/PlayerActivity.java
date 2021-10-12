package com.joyy.nativecpp;

import android.content.pm.ActivityInfo;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import com.joyy.nativecpp.databinding.ActivityPlayerBinding;
import java.io.IOException;

public class PlayerActivity extends AppCompatActivity {

    ActivityPlayerBinding binding;
    private MediaPlayer mMediaPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityPlayerBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


        initData();
    }

    private void initData() {
        binding.play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                play();
            }
        });
        binding.stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                stop();
            }
        });

        mMediaPlayer = new MediaPlayer();
        binding.mSurfaceView.getHolder().setKeepScreenOn(true);
        binding.mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
                //开始播放
                readyPlay();
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {

            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {

            }
        });

        mMediaPlayer.setOnVideoSizeChangedListener(new MediaPlayer.OnVideoSizeChangedListener() {
            @Override
            public void onVideoSizeChanged(MediaPlayer mediaPlayer, int i, int i1) {
                changeVideoSize();
            }
        });

    }


    public void changeVideoSize() {
        int videoWidth = mMediaPlayer.getVideoWidth();
        int videoHeight = mMediaPlayer.getVideoHeight();

        int surfaceWidth = binding.mSurfaceView.getWidth();
        int surfaceHeight = binding.mSurfaceView.getHeight();

        float max;
        if (getResources().getConfiguration().orientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) {
            //竖屏模式下按视频宽度计算放大倍数值
            max = Math.max((float) videoWidth / (float) surfaceWidth, (float) videoHeight / (float) surfaceHeight);
        } else {
            //横屏模式下按视频高度计算放大倍数值
            max = Math.max(((float) videoWidth / (float) surfaceHeight), (float) videoHeight / (float) surfaceWidth);
        }

        //视频宽高分别/最大倍数值 计算出放大后的视频尺寸
        videoWidth = (int) Math.ceil((float) videoWidth / max);
        videoHeight = (int) Math.ceil((float) videoHeight / max);

        //无法直接设置视频尺寸，将计算出的视频尺寸设置到surfaceView 让视频自动填充。
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(videoWidth, videoHeight);
        params.addRule(RelativeLayout.CENTER_VERTICAL, binding.mContainer.getId());
        binding.mSurfaceView.setLayoutParams(params);

    }

    public void readyPlay() {
        // String url = "http://res.cloudinary.com/liuyuesha/video/upload/v1475978853/广告_bl4dbp.mp4";
//        String url = "/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.mp4";
        mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
        try {
            mMediaPlayer.setDataSource("/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.mp4");
        } catch (Exception e) {
            e.printStackTrace();
        }
        mMediaPlayer.setLooping(true);
        // 把视频画面输出到SurfaceView
        mMediaPlayer.setDisplay(binding.mSurfaceView.getHolder());
        // 通过异步的方式装载媒体资源
        mMediaPlayer.prepareAsync();

        mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                //装载完毕回调
                //play();
                Toast.makeText(PlayerActivity.this, "mMediaPlayer.setOnPreparedListener", Toast.LENGTH_SHORT).show();
            }
        });
        mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mediaPlayer, int i, int i1) {
                Toast.makeText(PlayerActivity.this, "onError", Toast.LENGTH_SHORT).show();
                return false;
            }
        });
    }

    private void log(String msg) {
        Log.e(PlayerActivity.class.getSimpleName(), msg);
    }

    /**
     * 播放或者暂停
     */
    private void play() {
        log("play()");
        if (mMediaPlayer != null) {
            if (mMediaPlayer.isPlaying()) {
//                mMediaPlayer.pause();
//                mBtnPlay.setText("播放");
            } else {
                mMediaPlayer.start();
//                mBtnPlay.setText("暂停");
            }
        }
    }

    private void stop() {
        log("stop()");
        if (mMediaPlayer != null) {
            if (mMediaPlayer.isPlaying()) {
                mMediaPlayer.pause();
//                mBtnPlay.setText("播放");
            } else {
//                mMediaPlayer.start();
//                mBtnPlay.setText("暂停");
            }
        }
    }


    @Override
    protected void onDestroy() {
        if (mMediaPlayer != null) {
            if (mMediaPlayer.isPlaying()) {
                mMediaPlayer.stop();
            }
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
        super.onDestroy();
    }

}