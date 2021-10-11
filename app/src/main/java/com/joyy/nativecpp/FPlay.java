package com.joyy.nativecpp;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import androidx.annotation.NonNull;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Time:2021/9/29 15:59
 * Author: flannery
 * Description:
 */
public class FPlay extends GLSurfaceView implements SurfaceHolder.Callback,
        GLSurfaceView.Renderer, View.OnClickListener {

    static class CallbackImpl implements SurfaceHolder.Callback {
        @Override
        public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
            Log.d("XPlay", "surfaceCreated");
        }

        @Override
        public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
            Log.d("XPlay", "surfaceCreated");
        }

        @Override
        public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
            Log.d("XPlay", "surfaceCreated");
        }
    }

    static class RendererImpl implements Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int i, int i1) {

        }

        @Override
        public void onDrawFrame(GL10 gl10) {

        }
    }


    private SurfaceHolder.Callback mCallback;
    private GLSurfaceView.Renderer mRender;
    private OnClickListener mOnClickListener;

    public void setCallback(SurfaceHolder.Callback mCallback) {
        this.mCallback = mCallback;
    }

    public void setRender(Renderer mRender) {
        this.mRender = mRender;
    }

    public void setOnClickListener(OnClickListener mOnClickListener) {
        this.mOnClickListener = mOnClickListener;
    }

    public FPlay(Context context, AttributeSet attrs) {
        super(context, attrs);

        // android 8.9 需要设置
        setRenderer(this);
        setOnClickListener(this);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        if (mRender != null) mRender.onSurfaceCreated(gl10, eglConfig);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        if (mRender != null) mRender.onSurfaceChanged(gl10, i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        if (mRender != null) mRender.onDrawFrame(gl10);
    }

    @Override
    public void onClick(View view) {
        if (mOnClickListener != null) mOnClickListener.onClick(view);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        super.surfaceCreated(holder);
        if (mCallback != null) mCallback.surfaceCreated(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        super.surfaceChanged(holder, format, w, h);
        if (mCallback != null) mCallback.surfaceChanged(holder, format, w, h);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
        if (mCallback != null) mCallback.surfaceDestroyed(holder);
    }
}
