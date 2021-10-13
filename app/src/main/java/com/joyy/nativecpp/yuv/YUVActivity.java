package com.joyy.nativecpp.yuv;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.joyy.nativecpp.R;

import java.io.File;

public class YUVActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    // static {
    //     System.loadLibrary("nativecpp");
    // }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //去掉标题
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        //全屏.隐藏状态
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //横屏
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        ActivityCompat.requestPermissions(this,
            new String[]{
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
            }
            , 100);

        File file = new File(getExternalCacheDir(), "hello");
        file.mkdirs();

        setContentView(R.layout.activity_yuvactivity);
    }

}