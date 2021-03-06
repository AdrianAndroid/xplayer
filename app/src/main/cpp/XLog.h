//
// Created by 赵健 on 2021/9/27.
//

#ifndef NATIVECPP_XLOG_H
#define NATIVECPP_XLOG_H


class XLog {
};

#ifdef ANDROID

#include <android/log.h>

#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"XPlay",__VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,"XPlay",__VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"XPlay",__VA_ARGS__)
#else
#define XLOGD(...) printf("XPlay",__VA_ARGS__)
#define XLOGI(...) printf("XPlay",__VA_ARGS__)
#define XLOGE(...) printf("XPlay",__VA_ARGS__)
#endif


#endif //NATIVECPP_XLOG_H
