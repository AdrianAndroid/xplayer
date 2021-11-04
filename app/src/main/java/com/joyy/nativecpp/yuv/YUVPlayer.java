package com.joyy.nativecpp.yuv;

import android.annotation.SuppressLint;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.Toast;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Time:2021/10/13 15:30
 * Author: flannery
 * Description:
 */
public class YUVPlayer extends GLSurfaceView implements Runnable, SurfaceHolder.Callback {

    public YUVPlayer(Context context, AttributeSet attrs) {
        super(context, attrs);
        // 设置这个才能显示
        setRenderer(new Renderer() {
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                //gl.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            }

            public void onSurfaceChanged(GL10 gl, int w, int h) {
                //gl.glViewport(0, 0, w, h);
            }

            public void onDrawFrame(GL10 gl) {
                //gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
            }
        });
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.e("Xplay", "[YUVPlayer] surfaceCreated");
        new Thread(this).start();//回调run
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.e("Xplay", "[YUVPlayer] surfaceDestroyed");
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.e("Xplay", "[YUVPlayer] surfaceChanged");
    }

    @SuppressLint("SdCardPath")
    @Override
    public void run() {//传GLSurfaceView地址进去
        Log.e("XPlay", "thread name = " + Thread.currentThread().getName());
//    Open("/sdcard/outCat.yuv",getHolder().getSurface());
//    Open("/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.yuv",getHolder().getSurface());
//    Open("/sdcard/Android/data/yuvplayer.yuvplayer/cache/out.yuv",getHolder().getSurface());
        Open("/sdcard/Android/data/com.joyy.nativecpp/cache/out.yuv", getHolder().getSurface());
        // Open("/sdcard/Android/data/yuvplayer.yuvplayer/v1080.yuv",getHolder().getSurface());

    }

    public native void Open(String url, Object surface);


}
