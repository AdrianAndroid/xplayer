//
// Created by 赵健 on 2021/10/1.
//

#include "FFDemux.h"
#include "../XLog.h"

extern "C" {
#include <libavformat/avformat.h>
}

static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}


bool FFDemux::Open(const char *url) {
    XLOGI("FFDemux::Open file %s begin", url);
    Close();
    mux.lock();
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed!", url);
        return false;
    }
    XLOGI("FFDemux open %s success!", url);
    // 读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("avformat_find_stream_info %s failed!", url);
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 100);
    mux.unlock();
    XLOGI("total ms = %d!", totalMs);
    // 打印流信息
    // 注意：最后一个参数填0， 打印输入流：最后一个参数填1， 打印输出流
//    av_dump_format(ic, 0, url, 0);

    GetVPara();
    GetAPara();
    return true;
}

bool FFDemux::Seek(double pos) {
    if (pos < 0 || pos > 1) {
        XLOGE("Seek value must 0.0-1.0");
        return false;
    }
    bool re = false;
    mux.lock();
    if (!ic) {
        mux.unlock();
        return false;
    }
    // 清理读取的缓冲
    avformat_flush(ic);
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration * pos;

    // 往后跳转到关键帧
    re = av_seek_frame(ic, videoStream, seekPts, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return re;
}

void FFDemux::Close() {
    mux.lock();
    if (ic) {
        avformat_close_input(&ic);
    }
    mux.unlock();
}

XParameter FFDemux::GetVPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("[FFDemux] GetVPara failed! ic is NULL!");
        return XParameter();
    } else {
        XLOGI("[FFDemux] GetVPara succeed!");
    }
    // 获取了视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    } else {
        XLOGI("av_find_best_stream succeed!");
    }
    videoStream = re;
    XParameter para; // 创建栈内对象
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}

XParameter FFDemux::GetAPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("GetAPara failed! ic is NULL!");
        return XParameter();
    } else {
        XLOGI("GetAPara success!");
    }
    //获取了音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    } else {
        XLOGI("av_find_best_stream success!");
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}

XData FFDemux::Read() {
    return XData();
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
        //注册所有封装器
        av_register_all();
        XLOGI("register av_register_all!");
        //注册所有的解码器
        avcodec_register_all();
        XLOGI("register avcodec_register_all!");
        //初始化网络
        avformat_network_init();
        XLOGI("register avformat_network_init!");
    }
}
