package com.joyy.nativecpp;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Time:2021/10/13 10:31
 * Author: flannery
 * Description:
 */
public class XPlay extends GLSurfaceView {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);

        setRenderer(new Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int i, int i1) {

            }

            @Override
            public void onDrawFrame(GL10 gl10) {

            }
        });
    }
}
