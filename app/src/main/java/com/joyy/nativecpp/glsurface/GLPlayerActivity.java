package com.joyy.nativecpp.glsurface;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.joyy.nativecpp.databinding.ActivityGlplayerBinding;

public class GLPlayerActivity extends AppCompatActivity {

    ActivityGlplayerBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityGlplayerBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.mGLSurfaceView.setEGLContextClientVersion(2);
        binding.mGLSurfaceView.setRenderer(new GLVideoRenderer(this, ""));

    }
}