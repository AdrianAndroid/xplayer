package com.joyy.nativecpp;

import android.opengl.GLSurfaceView;
import android.util.Log;
import android.util.Printer;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import com.joyy.nativecpp.databinding.ActivityMainBinding;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * 学习JNI的知识
 */
public class MainActivity extends AppCompatActivity {

    // Used to load the 'nativecpp' library on application startup.
    // 使用本地库
    static {
        System.loadLibrary("nativecpp");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // 去掉标题栏
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        // 全屏，隐藏状态
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        initVideo();
        extracted();
    }


    private boolean isPlay = false;

    private void initVideo() {
        binding.mFPlay.setCallback(new FPlay.CallbackImpl() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
                super.surfaceCreated(surfaceHolder);
                initView(surfaceHolder.getSurface());
            }
        });

        binding.mFPlay.setRender(new FPlay.RendererImpl() {

        });

        binding.mFPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isPlay = !isPlay;
                if (isPlay)
                    play();
                else
                    pause();
            }
        });


        binding.btnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    String url = binding.mEditText.getText().toString();
                    open(url);
                    play();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        binding.btnPause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                pause();
            }
        });

        binding.btnStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                stop();
            }
        });

        binding.mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Log.d("XPlay", "progress = " + progress + " , fromUser = " + fromUser);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                Log.d("XPlay", "onStartTrackingTouch seekBar " + seekBar.getMax());
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d("XPlay", "onStopTrackingTouch seekBar " + seekBar.getMax());
                seek((double) seekBar.getProgress() / (double) seekBar.getMax());
            }
        });
    }

    private long mNativePlayId;

    // 打开url
    public native void open(String url);

    public native void seek(double pos);

    public native double playpos();

    public native void play();

    public native void pause();

    public native void stop();


    public void playCallBack(double pos) {

    }


    private void extracted() {
        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        Log.d("XPlay", "testJString\n\n");
        testJString(">>testJString!<<");

        Log.d("XPlay", "testIntArray\n\n");
        testIntArray(new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        testObjectArray(new String[]{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"});


        testCallJavaMethod();
        testCallStaticJavaMethod();
        Student student = new Student();
        Student.nickname = "static_nickname";//static String nickname;
        student.name = "_name";//String name;
        student.age = 11;//int age;
        Student.grade = 22;//static int grade;
        getJavaObjectField(student);

        Log.d("XPlay", "testJNIReference\n\n");
        testJNIReference(new Student());

        Log.d("XPlay", "testJavaException\n\n");
        try {
            testJavaException();
        } catch (Exception e) {
            Log.d("XPlay", "testJavaException 抛出了异常");
            e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'nativecpp' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void initView(Object surface);

    // 练习
    public native void testJString(String str);

    // JNI操作数组
    // 整型数组相关代码
    public native void testIntArray(int[] array);

    // Object Array 相关测试代码
    public native void testObjectArray(String[] strArr);


    // jni调用Java对象方法
    public native void testCallJavaMethod();

    // jni调用java static方法
    public native void testCallStaticJavaMethod();

    // jni访问java的对象属性和类属性
    public native void getJavaObjectField(Student student);

    public void helloworld(String msg) {
        Log.d("XPlay", "\n\n");
        Log.d("XPlay", "hello world:" + msg);
    }

    public static void helloworldStatic(String msg) {
        Log.d("XPlay", "\n\n");
        Log.d("XPlay", "Static hellow world:" + msg);
    }

    //4、JNI对象的全局引用和局部引用

    /**
     * 测试JNI强全局引用 和 弱全局引用
     */
    public native void testJNIReference(Object object);


    // 5.JNI进程间同步

    /**
     * JNI利用java对象对象进行线程同步
     *
     * @param lock
     */
    public native void testJNILock(Object lock);


    // 6. JNI异常相关的函数

    /**
     * 1. env->ExceptionOccurred() 判断JNI调用Java方法是否遇到了Exception
     * 2. env->ThrowNew()
     */
    public native void testJavaException();

    public void helloException() {
        Log.d("XPlay", "\n\n");
        Log.d("XPlay", "helloException");
    }


    // 四。、JNI和java对象的相互持有
    /**
     * 用户保存C++对象的引用
     */
    private long mNativeId;

    /**
     * Java对象持有C++对象
     */
    public native void initSDK();

    /**
     * Java对象释放C++对象
     */
    public native void releaseSDK();
}