#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "player/IPlayerProxy.h"
#include "XLog.h"
#include "Person.h"

// 测试用
#include <jni.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_joyy_nativecpp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    XLOGI("Hello from c++");
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testJString(JNIEnv *env, jobject thiz, jstring str_) {
    // （1）生成JNI String
    char const *str = "hello world!";
    jstring jstr = env->NewStringUTF(str);

    // (2) jstring 转换成 const char * charstr
    const char *charstr = env->GetStringUTFChars(str_, 0);
    XLOGI("[native-lib]  [testJString] env->GetStringUTFChars(str_, 0) -> %s ", charstr);
    // (3) 释放 const char *
    env->ReleaseStringUTFChars(str_, charstr);

    // (4) 获取字符串子集
    char *subStr = new char;
    env->GetStringUTFRegion(str_, 0, 3, subStr);
    XLOGI("[native-lib] env->GetStringUTFRegion(str_, 0, 3, subStr) -> %s", subStr);

    env->ReleaseStringUTFChars(str_, subStr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testIntArray(JNIEnv *env, jobject thiz, jintArray array_) {
    // ---- 获取数组元素
    // （1）获取数组中元素
    jint *intArray = env->GetIntArrayElements(array_, nullptr);
    int len = env->GetArrayLength(array_); //(2)获取数组长度
    XLOGI("[native-lib] len:%d", len);
    for (int i = 0; i < len; i++) {
        jint item = intArray[i];
        XLOGI("[native-lib] item[%d]:%d", i, item);
    }
    env->ReleaseIntArrayElements(array_, intArray, 0);

    // ---- 获取子数组
    jint *subArray = new jint;
    env->GetIntArrayRegion(array_, 0, 3, subArray);
    for (int i = 0; i < 3; i++) {
        subArray[i] = subArray[i] + 5;
        XLOGI("[native-lib] subArrya:[%d]:%d", i, subArray[i]);
    }

    // 用子数组修改原数组元素
    env->SetIntArrayRegion(array_, 0, 3, subArray);
    env->ReleaseIntArrayElements(array_, subArray, 0);//释放子数组元素
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testObjectArray(JNIEnv *env, jobject thiz,
                                                     jobjectArray str_arr) {
    // 获取数组长度
    int len = env->GetArrayLength(str_arr);
    for (int i = 0; i < len; i++) {
        //获取Object的数组元素
        jstring item = (jstring) env->GetObjectArrayElement(str_arr, i);

        const char *charStr = env->GetStringUTFChars(item, 0);
        XLOGI("[native-lib] strArray item:%s", charStr);

        jstring jresult = env->NewStringUTF("HaHa");
        // 设置Object数组元素
        env->SetObjectArrayElement(str_arr, i, jresult);
        env->ReleaseStringUTFChars(item, charStr);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testCallJavaMethod(JNIEnv *env, jobject thiz) {
    // 获取类名
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) return;

    jmethodID javaMethod = env->GetMethodID(clazz, "helloworld", "(Ljava/lang/String;)V");
    if (javaMethod == NULL) return;
    const char *msg = "nancy";
    jstring jmsg = env->NewStringUTF(msg);
    env->CallVoidMethod(thiz, javaMethod, jmsg);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testCallStaticJavaMethod(JNIEnv *env, jobject thiz) {
    // 获取java类型
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) return;

    jmethodID staticMethod = env->GetStaticMethodID(clazz, "helloworldStatic",
                                                    "(Ljava/lang/String;)V");
    if (staticMethod == NULL) return;

    jstring jmsg = env->NewStringUTF("wangfeng");
    env->CallStaticVoidMethod(clazz, staticMethod, jmsg);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_getJavaObjectField(JNIEnv *env, jobject thiz,
                                                        jobject student) {
    jclass clazz = env->GetObjectClass(student);
    if (clazz == NULL) return;

    // 获取Object实例属性
    jfieldID nameId = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    jstring jname = (jstring) env->GetObjectField(student, nameId);

    jfieldID ageId = env->GetFieldID(clazz, "age", "I");
    jint jage = env->GetIntField(student, ageId);

    const char *name = env->GetStringUTFChars(jname, 0);
    env->ReleaseStringUTFChars(jname, name);

    // 获取java类属性
    jfieldID gradId = env->GetStaticFieldID(clazz, "grade", "I");
    jint jgrade = env->GetStaticIntField(clazz, gradId);

    jfieldID nickeNameId = env->GetStaticFieldID(clazz, "nickname", "Ljava/lang/String;");
    jstring jnickname = (jstring) env->GetStaticObjectField(clazz, nickeNameId);

    const char *nickeName = env->GetStringUTFChars(jnickname, 0);
    env->ReleaseStringUTFChars(jnickname, nickeName);

    XLOGI("[native-lib] feifei: name:%s, age:%d, grade:%d, nickname:%s", name, jage, jgrade,
          nickeName);

    // JNI设置java对象属性
    env->SetObjectField(student, nameId, env->NewStringUTF("张三"));
    // JNI设置java类属性
    env->SetStaticObjectField(clazz, nickeNameId, env->NewStringUTF("小白"));
    jstring jnameNew = (jstring) env->GetObjectField(student, nameId);
    jstring jnickNameNew = (jstring) env->GetStaticObjectField(clazz, nickeNameId);

    const char *newName = env->GetStringUTFChars(jnameNew, 0);
    const char *newNickName = env->GetStringUTFChars(jnickNameNew, 0);

    env->ReleaseStringUTFChars(jnameNew, newName);
    env->ReleaseStringUTFChars(jnickNameNew, newNickName);
    XLOGI("[native-lib] feifei after update name:%s, age:%d, grade:%d, nickName:%s ", newName, jage,
          jgrade,
          newNickName);
}

/**
 * (1) 在JNI接口函数中引用JNIN对象的局部变量，都是对JNI对象的局部引用，所有这些JNI对象都会被自动释放。不过我们也可以采用JNI代码提供的DeleteLocalRef函数来删除一个局部JNI对象引用
 * (2) 对于JNI对象，绝对不能简单的声明一个全局变量，在JNI接口函数里面给这个全局变量赋值这么简单，一定要使用JNI代码提供的管理JNI对象的函数
 * JNI全局引用分为两种：一种全局引用，这种引用会阻止Java垃圾回收器回收JNI代码引用的对象；
 * 另一种是弱全局引用，这种全局引用不会阻止垃圾回收器回收JNI 代码引用的Java对象
 * -NewGlobalRef用来创建强全局引用的JNI对象
 * -DeleteGlobalRef来删除强全局引用的JNI对象
 * -NewWeakGlobalRef用来创建弱去啊句引用的JNI对象
 * -DeleteWeakGlobalRef用来删除弱全局引用的JNI对象
 * -IsSameObject用来判断两个JNI对象是否相同
 */
jobject gThiz;// 全局JNI对象引用
jobject gWeakThiz; //全局JNI对象弱引用
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testJNIReference(JNIEnv *env, jobject thiz, jobject object) {
    // 声明局部变量clazz
    jclass clazz = env->GetObjectClass(thiz);

    // 手动释放 局部变量 clazz； DeleteLocalRef 也可不用手动调用，JNI方法返回之后，会自动释放局部JNI变量
    env->DeleteLocalRef(clazz);

    // ----强全局变量
    gThiz = env->NewGlobalRef(object);//生成全局的JNI 对象引用，这样生成的全局的JNI对象 才可以在其他函数中使用

    env->DeleteGlobalRef(gThiz);// 在我们不需要gThis这个全局JNI对象引用时，可以将其删除

    // -----全局弱引用
    gWeakThiz = env->NewWeakGlobalRef(object);//生成全局的JNI对象引用， 这样生成的全局的JNI对象才可以在其它函数中使用

    if (env->IsSameObject(gWeakThiz, nullptr)) {
        XLOGI("[native-lib] 全局引用 已经被释放了");
    } else {
        XLOGI("[native-lib] 全局引用 还没有被释放");
    }
    // 释放 全局弱引用对象
    env->DeleteWeakGlobalRef(gWeakThiz);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testJNILock(JNIEnv *env, jobject thiz, jobject lock) {
    // 加锁
    env->MonitorEnter(lock);
    // doSomething
    XLOGI("[native-lib] feifei, this is in lock");
    // 释放锁
    env->MonitorExit(lock);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testJavaException(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) return;
    jmethodID helloException_method = env->GetMethodID(clazz, "helloException", "()V");
    if (helloException_method == NULL) return;
    env->CallVoidMethod(thiz, helloException_method);
    if (env->ExceptionOccurred() != NULL) {
        //env->ExceptionDescribe();
        env->ExceptionClear();
        XLOGI("[native-lib] feifei, 调用java方法时 遇到了Excpetion");
        return;
    } else {
        XLOGI("[native-lib] feifei, 调用java方法时 没有遇到Excpetion");
    }
    XLOGI("[native-lib] feifei, 调用helloException 方法成功了!");

    XLOGI("[native-lib] feifei, now JNI throw java exception - beging");
    jclass expectionClazz = env->FindClass("java/lang/Exception");
    if (expectionClazz == NULL) return;
    env->ThrowNew(expectionClazz, "this is a exception");
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_initSDK(JNIEnv *env, jobject thiz) {
    Person *person = new Person();
    person->setAge(18);
    person->initSDK();

    jclass classzz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(classzz, "mNativeId", "J");

    // 将C++对象的地址绑定到Java变量中
    env->SetLongField(thiz, fid, (jlong) person);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_releaseSDK(JNIEnv *env, jobject thiz) {
    jclass objectClass = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(objectClass, "mNativeId", "J");

    // 取出java对象中保存的C++对象地址
    jlong p = env->GetLongField(thiz, fid);

    //转换成C++对象
    Person *person = (Person *) p;
    person->releaseSDK();
    //释放person C++对象
    free(person);
    env->SetLongField(thiz, fid, (jlong) -1);//设置成默认是
}








extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    XLOGI("JNI_OnLoad");
    IPlayerProxy::Get()->Init(vm);
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    XLOGI("JNI_OnUnload");
}


extern "C"
JNIEXPORT jdouble JNICALL
Java_com_joyy_nativecpp_MainActivity_playpos(JNIEnv *env, jobject thiz) {
    XLOGI("[native-lib] playpos ");
    return IPlayerProxy::Get()->PlayPos();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_play(JNIEnv *env, jobject thiz) {
    XLOGI("[native-lib] play ");
    IPlayerProxy::Get()->SetPause(true);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_pause(JNIEnv *env, jobject thiz) {
    XLOGI("[native-lib] pause ");
    IPlayerProxy::Get()->SetPause(false);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_stop(JNIEnv *env, jobject thiz) {
    XLOGI("[native-lib] stop ");
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_open(JNIEnv *env, jobject thiz, jstring url) {
    // 生成JNI String
    //    // (2) jstring 转换成 const char * charstr
    //    const char *charstr = env->GetStringUTFChars(str_, 0);
    //    XLOGI("[native-lib] [testJString] env->GetStringUTFChars(str_, 0) -> %s ", charstr);
    //    // (3) 释放 const char *
    //    env->ReleaseStringUTFChars(str_, charstr);
    // 得到字符串
    const char *_url = env->GetStringUTFChars(url, 0);
    XLOGI("[native-lib] oepn %s", _url);
    IPlayerProxy::Get()->Open(_url); // 打开文件，并解码
    XLOGI("[native-lib] Start %s", _url);
    IPlayerProxy::Get()->Start();

    env->ReleaseStringUTFChars(url, _url);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::Get()->InitView(win);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_seek(JNIEnv *env, jobject thiz, jdouble pos) {
    XLOGI("[native-lib] SEEK %f ", pos);
}



//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// 测试音频
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

// 1. 创建引擎
static SLObjectItf engineSL = NULL;

SLEngineItf CreateSL() {
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    XLOGI("slCreateEngine success!!");
    if (re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    XLOGI("(*engineSL)->Realize success!!");
    if (re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    XLOGI("(*engineSL)->GetInterface success!!");
    if (re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {
    XLOGI("PcmCall");
    static FILE *fp = NULL;
    static char *buf = NULL;
    if (!buf) buf = new char[1024 * 1024];
    const char *url = "/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.pcm";
    //r     以只读方式打开文件，该文件必须存在。
    //r+    以读/写方式打开文件，该文件必须存在。
    //rb+   以读/写方式打开一个二进制文件，只允许读/写数据。
    //rt+   以读/写方式打开一个文本文件，允许读和写。
    //w     打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
    //w+    打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
    //a     以附加的方式打开只写文件。若文件不存在，则会创建该文件；如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF 符保留）。
    //a+    以附加方式打开可读/写的文件。若文件不存在，则会创建该文件，如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF符不保留）。
    //wb    以只写方式打开或新建一个二进制文件，只允许写数据。
    //wb+   以读/写方式打开或新建一个二进制文件，允许读和写。
    //wt+   以读/写方式打开或新建一个文本文件，允许读和写。
    //at+   以读/写方式打开一个文本文件，允许读或在文本末追加数据。
    //ab+   以读/写方式打开一个二进制文件，允许读或在文件末追加数据。
    if (!fp) {
        fp = fopen(url, "rb+");
        if (!fp) {
            XLOGE("open %s failed!", url);
            return;
        } else {
            XLOGI("open %s success!", url);
        }
    }
    if (feof(fp) == 0) {
        XLOGE("feof(fp) == 0");
        int len = fread(buf, 1, 1024, fp);
        if (len > 0) (*bf)->Enqueue(bf, buf, len);
    } else {
        XLOGE("feof(fp) != 0");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testAudio(JNIEnv *env, jobject thiz, jstring url) {
    // 得到字符串
//    const char *_url = env->GetStringUTFChars(url, 0);
//    XLOGI("[native-lib] testAudio url = %s", _url);
//    env->ReleaseStringUTFChars(url, _url);

    // 测试打开文件
    const char *m_url = "/sdcard/Android/data/com.joyy.nativecpp/cache/v1080.pcm";
    FILE *fp = fopen(m_url, "rb");
    if (fp) {
        XLOGI("test %s file open success ", m_url);
        fclose(fp);
    } else {
        XLOGE("test %s file open failed! with %d , %s", m_url, errno, strerror(errno));
    }

    // 1. 创建引擎
    SLEngineItf eng = CreateSL();
    if (eng) {
        XLOGI("CreateSL success!");
    } else {
        XLOGE("CreateSL failed!");
        return;
    }

    // 2. 创建混音器
    SLObjectItf mix = NULL;
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*eng)->CreateOutputMix failed!");
        return;
    } else {
        XLOGI("(*eng)->CreateOutputMix success!");
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*mix)->Realize failed!");
        return;
    } else {
        XLOGI("(*mix)->Realize success!");
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};

    // 3. 配置音频信息
    // 缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    // 音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2, //声道数
            SL_SAMPLINGRATE_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，
    };
    SLDataSource ds = {&que, &pcm};

    // 4. 创建播放器
    SLObjectItf player = NULL;
    SLPlayItf iplayer = NULL;
    SLAndroidSimpleBufferQueueItf pcmQue = NULL;
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*eng)->CreateAudioPlayer failed!");
    } else {
        XLOGI("(*eng)->CreateAudioPlayer success!");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*eng)->Realize failed!");
    } else {
        XLOGI("(*eng)->Realize success!");
    }
    // 获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*eng)->GetInterface SL_IID_PLAY failed!");
    } else {
        XLOGI("(*eng)->GetInterface SL_IID_PLAY success!");
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("(*eng)->GetInterface SL_IID_BUFFERQUEUE failed!");
    } else {
        XLOGI("(*eng)->GetInterface SL_IID_BUFFERQUEUE success!");
    }

    if (!pcmQue) {
        XLOGE("pcmQue 队列 failed!");
    } else {
        XLOGI("pcmQue 队列 success!");
    }

    if (!iplayer) {
        XLOGE("iplayer  failed!");
    } else {
        XLOGI("iplayer  success!");
    }

    // 设置回调函数， 播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue, PcmCall, 0);

    //启动播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);

    // 启动队列回调
    (*pcmQue)->Enqueue(pcmQue, "", 1);
}




//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// 测试视频
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testVideo(JNIEnv *env, jobject thiz, jstring url) {
    const char *_url = env->GetStringUTFChars(url, 0);
    XLOGI("[native-lib] testVideo url = %s", _url);
    env->ReleaseStringUTFChars(url, _url);
}