//
// Created by 赵健 on 2021/9/27.
//

#include "IPlayer.h"
#include "../XLog.h"
#include "../video/IVideoView.h"
#include "../resample/IResample.h"
#include "../audio/IAudioPlay.h"
#include "../demux/IDemux.h"
#include "../decode/IDecode.h"


IPlayer *IPlayer::Get(unsigned char index) {
    XLOGI("IPlayer *IPlayer::Get index=%d", index);
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    XLOGI("[IPlayer] %s", path);
    Close();
    mux.lock();
    XLOGI("IPlayer::Open 解封装");
    XLOGI("IPlayer::Open dexmux=%p", &demux);
    // 解封装
    if (!demux || !demux->Open(path)) {
        mux.unlock();
        XLOGE("demux->Open %s failed! ! demux is null or demux->Open not ok!", path);
        return false;
    } else {
        XLOGI("demux->Open %s sucess!", path);
    }
    // 解码 解码可能不需要，如果是解封之后就是原始数据
    XLOGI("解码 解码可能不需要，如果是解封之后就是原始数据");
    if (!vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode)) {
        XLOGE("vdecode->Open %s failed!", path);
    } else {
        XLOGE("vdecode->Open %s success!", path);
    }
    if (!adecode || !adecode->Open(demux->GetAPara())) {
        XLOGE("adecode->Open %s failed!", path);
    } else {
        XLOGE("adecode->Open %s success!", path);
    }

    // 重采样 有可能不需要， 解码后或者解封后可能直接能播放的数据
//    if(outPara.sample_rate <= 0)
    outPara = demux->GetAPara();
    if (!resample || !resample->Open(demux->GetAPara(), outPara)) {
        XLOGE("resample->Open %s failed!", path);
    }

    mux.unlock();
    return true;
}

void IPlayer::Close() {
    XLOGI("[IPlayer] Close");
    // 2 先关闭主体线程， 再清理观察者
    // 同步线程
    XThread::Stop();
    // 解封装
    if (demux) demux->Stop();
    // 解码
    if (vdecode) vdecode->Stop();
    if (adecode) adecode->Stop();
    if (audioPlay) audioPlay->Stop();

    // 2. 清理缓冲队列
    if (vdecode) vdecode->Clear();
    if (adecode) adecode->Clear();
    if (audioPlay) audioPlay->Clear();


    // 3。清理资源
    if (audioPlay) audioPlay->Close();
    if (videoView) videoView->Close();
    if (vdecode) vdecode->Close();
    if (adecode) adecode->Close();
    if (demux) demux->Close();
    mux.unlock();
}

bool IPlayer::Start() {
    XLOGI("[IPlayer] Start");
    mux.lock();

    if (vdecode) {
        vdecode->Start(); // 视频解码
        XLOGI("vdecode->Start() success!");
    } else {
        XLOGI("vdecode->Start() failed!");
    }

    if (!demux || !demux->Start()) {
        mux.unlock();
        XLOGE("demux->Start failed!");
        return false;
    } else {
        XLOGE("demux->Start success!");
    }
    if (adecode) {
        adecode->Start(); // 开始音频解码
        XLOGE("adecode->Start() success!");
    } else {
        XLOGE("adecode->Start() failed!");
    }
    if (audioPlay) {
        audioPlay->StartPlay(outPara); // 破榜音频
        XLOGI("aduioPlay->StartPlaye success!");
    } else {
        XLOGI("aduioPlay->StartPlaye failed!");
    }
    XThread::Start();

    mux.unlock();
    return false;
}

void IPlayer::InitView(void *win) {
    XLOGI("[IPlayer] InitView");
    if (videoView) {
        videoView->Close();
        videoView->SetRender(win);
        XLOGI("viewView is SUCCESS!");
    } else {
        XLOGE("video view is NULL");
    }
}

double IPlayer::PlayPos() {
    XLOGI("[IPlayer] PlayPos");
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if (demux) total = demux->totalMs; // 解封装的时候得到的
    if (total > 0) {
        if (vdecode) pos = (double) vdecode->pts / (double) total;
    }
    mux.unlock();
    return pos;
}

bool IPlayer::Seek(double pos) {
    XLOGI("[IPlayer] Seek pos=%f", pos);
    bool re = false;
    if (!demux) return false;
    // 暂停所有线程
    SetPause(true);
    mux.lock();
    // 清理缓冲
    // 2. 清理缓冲队列
    if (vdecode) vdecode->Clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if (adecode) adecode->Clear();
    if (audioPlay) audioPlay->Clear();

    re = demux->Seek(pos); //seek跳转到关键帧
    if (!vdecode) {
        mux.unlock();
        setPause(false);
        return re;
    }
    //解码到实际需要显示的帧
    int seekPts = pos * demux->totalMs;
    while (!isExit) {
        XData pkt = demux->Read();
        if (pkt.size <= 0) break;
        if (pkt.isAudio) {
            if (pkt.pts < seekPts) {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            demux->Notify(pkt);
            continue;
        }

        // 解码需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        XData data = vdecode->RecvFrame();
        if (data.size <= 0) continue;
        if (data.pts >= seekPts) break;
    }
    mux.unlock();
    setPause(false);
    return false;
}

void IPlayer::SetPause(bool isP) {
    if (isP) {
        XLOGI("[IPlayer] Seek SetPause true");
    } else {
        XLOGI("[IPlayer] Seek SetPause false");
    }
    mux.lock();
    XThread::setPause(isP);
//    if(demux) demux->SetPause(isP);
//    if(vdecode) vdecode->SetPause(isP);
//    if(adecode) adecode->SetPause(isP);
//    if(audioPlay) audioPlay->SetPause(isP);
    mux.unlock();
}

void IPlayer::Main() {
    while (!isExit) {
        mux.lock();
        if (!audioPlay || !vdecode) {
            mux.unlock();
            XSleep(2);
            continue;
        }

        // 同步
        // 获取音频的pts 告诉视频
        int apts = audioPlay->pts;
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }
}
