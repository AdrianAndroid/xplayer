package com.joyy.nativecpp;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.TextView;

import com.joyy.nativecpp.databinding.ActivityMainBinding;
import com.joyy.nativecpp.glsurface.GLPlayerActivity;
import com.joyy.nativecpp.yuv.YUVActivity;

import java.io.File;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

/**
 * 学习JNI的知识
 * https://blog.csdn.net/u010141160/article/details/83001142
 */
public class MainActivity extends AppCompatActivity {

    // Used to load the 'nativecpp' library on application startup.
    // 使用本地库
    static {
        Log.i("XPlay", " System.loadLibrary(\"nativecpp\");");
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

//        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
        ActivityCompat.requestPermissions(this,
            new String[]{
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
            }
            , 100);
//        }

        // 播放YUV视频
        initVideo1();
        // initVideo();
//        extracted();

        // initMediaPlayer();

        // initTest();

        File file = new File(getExternalCacheDir(), "hello");
        file.mkdirs();

        // 测试JNI方法
        binding.btnJni.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                extracted();
            }
        });
        // 播放音频
        binding.btnAudio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testAudio2();
            }
        });
        // 播放视频
        binding.btnYUV.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, YUVActivity.class));
            }
        });
        binding.btnFFmpeg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                test039();
            }
        });
    }

    private void initVideo1() {

        binding.btnOpenLocal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                open(binding.etLocal.getText().toString());
            }
        });

        binding.mXPlayer.setMCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {
                initView(holder.getSurface());
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
    }

    // private final MediaPlayer mMediaPlayer = new MediaPlayer();
    //
    // private void initMediaPlayer() {
    //     binding.mSurfaceView.setEGLContextClientVersion(2);
    //     binding.mSurfaceView.getHolder().setKeepScreenOn(true);
    //     binding.mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
    //         @Override
    //         public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
    //             readyPlay();
    //             Toast.makeText(MainActivity.this, "readyPlay() done", Toast.LENGTH_SHORT);
    //         }
    //
    //         @Override
    //         public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    //
    //         }
    //
    //         @Override
    //         public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
    //
    //         }
    //     });
    //     mMediaPlayer.setOnVideoSizeChangedListener(new MediaPlayer.OnVideoSizeChangedListener() {
    //         @Override
    //         public void onVideoSizeChanged(MediaPlayer mediaPlayer, int i, int i1) {
    //             //changeVideoSize();
    //         }
    //     });
    //     binding.play.setOnClickListener(new View.OnClickListener() {
    //         @Override
    //         public void onClick(View view) {
    //             play222();
    //         }
    //     });
    //
    //     binding.play.setOnClickListener(new View.OnClickListener() {
    //         @Override
    //         public void onClick(View view) {
    //             stop222();
    //         }
    //     });
    // }
    //
    // public void changeVideoSize() {
    //     int videoWidth = mMediaPlayer.getVideoWidth();
    //     int videoHeight = mMediaPlayer.getVideoHeight();
    //
    //     int surfaceWidth = binding.mSurfaceView.getWidth();
    //     int surfaceHeight = binding.mSurfaceView.getHeight();
    //
    //     float max;
    //     if (getResources().getConfiguration().orientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) {
    //         //竖屏模式下按视频宽度计算放大倍数值
    //         max = Math.max((float) videoWidth / (float) surfaceWidth, (float) videoHeight / (float) surfaceHeight);
    //     } else {
    //         //横屏模式下按视频高度计算放大倍数值
    //         max = Math.max(((float) videoWidth / (float) surfaceHeight), (float) videoHeight / (float) surfaceWidth);
    //     }
    //
    //     //视频宽高分别/最大倍数值 计算出放大后的视频尺寸
    //     videoWidth = (int) Math.ceil((float) videoWidth / max);
    //     videoHeight = (int) Math.ceil((float) videoHeight / max);
    //
    //     //无法直接设置视频尺寸，将计算出的视频尺寸设置到surfaceView 让视频自动填充。
    //     RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(videoWidth, videoHeight);
    //     params.addRule(RelativeLayout.CENTER_VERTICAL, binding.mContainer.getId());
    //     binding.mSurfaceView.setLayoutParams(params);
    //
    // }
    //
    // public void readyPlay() {
    //     mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
    //     try {
    //         mMediaPlayer.setDataSource("/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.mp4");
    //     } catch (Exception e) {
    //         e.printStackTrace();
    //     }
    //     mMediaPlayer.setLooping(true);
    //     // 把视频画面输出到SurfaceView
    //     mMediaPlayer.setDisplay(binding.mSurfaceView.getHolder());
    //     // 通过异步的方式装载媒体资源
    //     mMediaPlayer.prepareAsync();
    //
    //     mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
    //         @Override
    //         public void onPrepared(MediaPlayer mp) {
    //             //装载完毕回调
    //             play222();
    //             Toast.makeText(MainActivity.this, "mMediaPlayer.setOnPreparedListener", Toast.LENGTH_SHORT).show();
    //         }
    //     });
    //     mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
    //         @Override
    //         public boolean onError(MediaPlayer mediaPlayer, int i, int i1) {
    //             Toast.makeText(MainActivity.this, "onError", Toast.LENGTH_SHORT).show();
    //             return false;
    //         }
    //     });
    // }
//
//     /**
//      * 播放或者暂停
//      */
//     private void play222() {
//         if (mMediaPlayer != null) {
//             if (mMediaPlayer.isPlaying()) {
// //                mMediaPlayer.pause();
// //                mBtnPlay.setText("播放");
//             } else {
//                 mMediaPlayer.start();
// //                mBtnPlay.setText("暂停");
//             }
//         }
//     }
//
//     private void stop222() {
//         if (mMediaPlayer != null) {
//             if (mMediaPlayer.isPlaying()) {
//                 mMediaPlayer.pause();
// //                mBtnPlay.setText("播放");
//             } else {
// //                mMediaPlayer.start();
// //                mBtnPlay.setText("暂停");
//             }
//         }
//     }

    //OpenGlES sharder初始化完成并编译顶点和着色器代码
    public native void test57(Object surface);

    //OpenglES program 渲染程序初始化给shader传递顶点和材质顶点数据
    public native void test59(Object surface);

    //OpenglES opengl的yuv纹理的创建和初始化
    public native void test60(Object surface);

    //OpenglES opengl纹理数据修改和显示
    public native void test61(Object surface);

    //OpenglES 通过 opengl纹理修改完成yuv文件的播放显示
    public native void test62(Object surface);

    private void initTest() {

        //视频
        binding.test57.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        test57(getSurface());
                    }
                }).start();
            }
        });
        binding.test59.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, PlayerActivity.class));
            }
        });
        binding.test60.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, GLPlayerActivity.class));
            }
        });
        binding.test61.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, YUVActivity.class));
            }
        });
        binding.test62.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test62(getSurface());
            }
        });

        // 音频
        findViewById(R.id.testAudio).setOnClickListener(new View.OnClickListener() {
            @SuppressLint("SdCardPath")
            @Override
            public void onClick(View view) {
                testAudio("/sdcard/audio.pcm");
            }
        });

        findViewById(R.id.testAudio2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                testAudio2();
            }
        });
        findViewById(R.id.releaseAudio).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                releaseAudio2();
            }
        });

        findViewById(R.id.testVideo).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                testVideo(binding.etLocal.getText().toString());
            }
        });
        findViewById(R.id.test027).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test027();// 通过遍历获取AVStream音视频流信息并打印参数
            }
        });
        findViewById(R.id.test032).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test032();// av_read_frame和av_seek_frame
            }
        });
        findViewById(R.id.test033).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test033();// av_read_frame和av_seek_frame
            }
        });
        findViewById(R.id.test034).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test034();// av_read_frame和av_seek_frame
            }
        });
        findViewById(R.id.test037).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test037();// av_read_frame和av_seek_frame
            }
        });
        findViewById(R.id.test039).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                test039();// av_read_frame和av_seek_frame
            }
        });
    }

    private Surface getSurface() {
        return null;// binding.mSurfaceView.getHolder().getSurface();
    }

    public native void test027();

    // av_read_frame和av_seek_frame
    public native void test032();

    // AVCodecContext解码上下文初始化
    public native void test033();

    //打开音频解码器上下文
    public native void test034();

    //完成音视频解码
    public native void test037();

    //ffmpeg调用MediaCodec实现硬解码代码演示
    public native void test039();

    private final boolean isPlay = false;

    private void initVideo() {


        // binding.mFPlay.setCallback(new FPlay.CallbackImpl() {
        //     @Override
        //     public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        //         super.surfaceCreated(surfaceHolder);
        //         Log.d("XPlay", "surfaceCreated");
        //         //initView(surfaceHolder.getSurface());
        //     }
        // });

        // binding.mFPlay.setRender(new FPlay.RendererImpl() {
        //
        // });

        // binding.mFPlay.setOnClickListener(new View.OnClickListener() {
        //     @Override
        //     public void onClick(View view) {
        //         isPlay = !isPlay;
        //         if (isPlay) {
        //             play();
        //         } else {
        //             pause();
        //         }
        //     }
        // });


        binding.btnOpenRTMP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                open(binding.etRTMP.getText().toString());
            }
        });

        binding.btnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
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

    // 测试音频
    public native void testAudio(String url);

    public native void testAudio2();

    public native void releaseAudio2();

    // 测试视频
    public native void testVideo(String url);

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
        // 测试传递字符串
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        Log.d("XPlay", "testJString\n\n");
        testJString(">>testJString!<<");

        // 测试传递数组(int数组 和 字符串数组)
        Log.d("XPlay", "testIntArray\n\n");
        testIntArray(new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        testObjectArray(new String[]{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"});

        // jni调用Java对象方法
        testCallJavaMethod();
        // jni调用Java静态方法
        testCallStaticJavaMethod();
        // java向jni传递对象
        Student student = new Student();
        Student.nickname = "static_nickname";//static String nickname;
        student.name = "_name";//String name;
        student.age = 11;//int age;
        Student.grade = 22;//static int grade;
        getJavaObjectField(student);

        // 强全局引用 和 弱全局引用
        Log.d("XPlay", "testJNIReference\n\n");
        testJNIReference(new Student());

        // jni抛出java异常
        Log.d("XPlay", "testJavaException\n\n");
        try {
            testJavaException();
        } catch (Exception e) {
            Log.d("XPlay", "testJavaException 抛出了异常");
            e.printStackTrace();
        }

        // 本地持有句柄, mNativeId为保存的句柄
        initSDK();
        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // 延迟销毁
                releaseSDK();
            }
        }, 2000);
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