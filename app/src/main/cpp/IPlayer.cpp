//
// Created by 赵健 on 2021/9/27.
//

#include "IPlayer.h"
#include "XLog.h"
#include "demux/IDemux.h"

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
    if(!demux || !demux->Open(path)){
        mux.unlock();
        XLOGE("demux->Open %s failed! ! demux is null or demux->Open not ok!", path);
        return false;
    } else {
        XLOGI("demux->Open %s sucess!", path);
    }
    // 解码 解码可能不需要，如果是解封之后就是原始数据
    XLOGI("解码 解码可能不需要，如果是解封之后就是原始数据");
    mux.unlock();
    return true;
}

void IPlayer::Close() {
    XLOGI("[IPlayer] Close");
}

bool IPlayer::Start() {
    XLOGI("[IPlayer] Start");
    return false;
}

void IPlayer::InitView(void *win) {
    XLOGI("[IPlayer] InitView");
}

double IPlayer::PlayPos() {
    XLOGI("[IPlayer] PlayPos");
    return 0;
}

bool IPlayer::Seek(double pos) {
    XLOGI("[IPlayer] Seek pos=%f", pos);
    return false;
}

void IPlayer::SetPause(bool isP) {
    if (isP) {
        XLOGI("[IPlayer] Seek SetPause true");
    } else {
        XLOGI("[IPlayer] Seek SetPause false");
    }
}

void IPlayer::Main() {

}
