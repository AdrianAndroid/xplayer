//
// Created by 赵健 on 2021/10/8.
//

#include "FFDecode.h"
#include "../XLog.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}


void FFDecode::InitHard(void *vm) {
    av_jni_set_java_vm(vm, 0);
}

bool FFDecode::Open(XParameter para, bool isHard) {
    Close();
    if (!para.para) return false;
    AVCodecParameters *p = para.para;
    // 1. 查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (isHard) { // 硬解码，找编码器
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if (!cd) {
        XLOGE("avcodec_find_decoder %d failed! %d", p->codec_id, isHard);
        return false;
    } else {
        XLOGE("avcodec_find_decoder %d success!", p->codec_id);
    }
    mux.lock();
    // 2. 创建编码上下文， 并复制参数
    codec = avcodec_alloc_context3(cd);
    if(!codec) {
        XLOGE("avcodec_alloc_context3 failed");
    } else {
        XLOGI("avcodec_alloc_context3 success!");
    }
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8; //编解码的线程数
    // 3. 打开编码器
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("avcodec_open2 failed! %s", buf);
    } else {
        XLOGE("avcodec_open2 success!");
    }
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        this->isAudio = true;
    }
    mux.unlock();
    XLOGI("avcodec_open2 success!");
    return false;
}

void FFDecode::Close() {
    IDecode::Clear();
    mux.lock();
    pts = 0;
    if (frame) av_frame_free(&frame);
    if (codec) {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

void FFDecode::Clear() {
    IDecode::Clear();
    mux.lock();
    if (codec) avcodec_flush_buffers(codec);
    mux.unlock();
}

bool FFDecode::SendPacket(XData pkt) {
    if (pkt.size <= 0 || !pkt.data)
    {
        XLOGI("FFDecode::SendPacket pkt.size <= 0 || !pkt.data");
        return false;
    }
    mux.lock();
    if (!codec) {
        XLOGI("FFDecode::SendPacket !codec");
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, (AVPacket *) pkt.data);
    mux.unlock();
    if (re != 0) {
        XLOGI("FFDecode::SendPacket re != 0, %s", strerror(re));
        return false;
    }
    return true;
}

XData FFDecode::RecvFrame() {
    mux.lock();
    if (!codec) {
        mux.unlock();
        XLOGE("codec 初始化 failed!");
        return XData();
    } else {
        XLOGE("codec 初始化 成功!");
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        mux.unlock();
        XLOGE("avcodec_receive_frame failed!");
        return XData();
    } else {
        XLOGE("avcodec_receive_frame success!");
    }
    XData d;
    d.data = (unsigned char *) frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2] * frame->height);
        d.width = frame->width;
        d.height = frame->height;
    } else {
        // 样本数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat) frame->format) * frame->nb_samples * 2;
    }
    d.format = frame->format;
    memcpy(d.datas, frame->data, sizeof(d.datas));
    d.pts = frame->pts;
    pts = d.pts;
    mux.unlock();
    return d;
}

// overrides a member function but is not marked 'override'
// -Winconsistent-missing-override
void FFDecode::Update(XData pkt) {
    IDecode::Update(pkt);
}

bool FFDecode::Start() {
    return XThread::Start();
}

void FFDecode::Stop() {
    XThread::Stop();
}

void FFDecode::setPause(bool isP) {
    XThread::setPause(isP);
}

bool FFDecode::IsPause() {
    return XThread::IsPause();
}

void FFDecode::Main() {
    IDecode::Main();
}

