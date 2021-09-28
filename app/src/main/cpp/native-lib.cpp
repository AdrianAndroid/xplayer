#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "IPlayerProxy.h"
#include "XLog.h"
#include "Person.h"

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
Java_com_joyy_nativecpp_MainActivity_open(JNIEnv *env, jobject thiz, jstring url) {
    // 生成JNI String
    const char *str = env->GetStringUTFChars(url, 0);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testJString(JNIEnv *env, jobject thiz, jstring str_) {
    // （1）生成JNI String
    char const *str = "hello world!";
    jstring jstr = env->NewStringUTF(str);

    // (2) jstring 转换成 const char * charstr
    const char *charstr = env->GetStringUTFChars(str_, 0);
    XLOGI("[testJString] env->GetStringUTFChars(str_, 0) -> %s ", charstr);
    // (3) 释放 const char *
    env->ReleaseStringUTFChars(str_, charstr);

    // (4) 获取字符串子集
    char *subStr = new char;
    env->GetStringUTFRegion(str_, 0, 3, subStr);
    XLOGI("env->GetStringUTFRegion(str_, 0, 3, subStr) -> %s", subStr);

    env->ReleaseStringUTFChars(str_, subStr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_joyy_nativecpp_MainActivity_testIntArray(JNIEnv *env, jobject thiz, jintArray array_) {
    // ---- 获取数组元素
    // （1）获取数组中元素
    jint *intArray = env->GetIntArrayElements(array_, nullptr);
    int len = env->GetArrayLength(array_); //(2)获取数组长度
    XLOGI("len:%d", len);
    for (int i = 0; i < len; i++) {
        jint item = intArray[i];
        XLOGI("item[%d]:%d", i, item);
    }
    env->ReleaseIntArrayElements(array_, intArray, 0);

    // ---- 获取子数组
    jint *subArray = new jint;
    env->GetIntArrayRegion(array_, 0, 3, subArray);
    for (int i = 0; i < 3; i++) {
        subArray[i] = subArray[i] + 5;
        XLOGI("subArrya:[%d]:%d", i, subArray[i]);
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
        XLOGI("strArray item:%s", charStr);

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

    XLOGI("feifei: name:%s, age:%d, grade:%d, nickname:%s", name, jage, jgrade, nickeName);

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
    XLOGI("feifei after update name:%s, age:%d, grade:%d, nickName:%s ", newName, jage, jgrade,
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
        XLOGI("全局引用 已经被释放了");
    } else {
        XLOGI("全局引用 还没有被释放");
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
    XLOGI("feifei, this is in lock");
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
        XLOGI("feifei, 调用java方法时 遇到了Excpetion");
        return;
    } else {
        XLOGI("feifei, 调用java方法时 没有遇到Excpetion");
    }
    XLOGI("feifei, 调用helloException 方法成功了!");

    XLOGI("feifei, now JNI throw java exception - beging");
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