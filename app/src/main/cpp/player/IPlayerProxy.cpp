//
// Created by 赵健 on 2021/9/27.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"
#include "../XLog.h"

bool IPlayerProxy::Open(const char *path) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Open %s  player=%p", path, player);
    bool re = false;
    mux.lock();
    if (player) {
        player->isHardDecode = isHardDecode;
        XLOGI("IPlayerProxy::Open player is not null player=%p", &player);
        re = player->Open(path);
        XLOGI("IPlayerProxy::Open player is not null");
    } else {
        XLOGI("IPlayerProxy::Open player is null");
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::Seek(double pos) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Seek %f", pos);
    bool re = false;
    mux.lock();
    if (player) re = player->Seek(pos);
    mux.unlock();
    return re;
}

void IPlayerProxy::Close() {
    XLOGI("[IPlayerProxy] IPlayerProxy::Close");
    IPlayer::Close();
}

bool IPlayerProxy::Start() {
    XLOGI("[IPlayerProxy] IPlayerProxy::Start");
    bool re = false;
    mux.lock();
    if (player) re = player->Start();
    mux.unlock();
    return re;
}

void IPlayerProxy::InitView(void *win) {
    XLOGI("[IPlayerProxy] IPlayerProxy::InitView");
    mux.lock();
    if (player) {
        player->InitView(win);
    }
    mux.unlock();
}

void IPlayerProxy::SetPause(bool isP) {
    XLOGI("[IPlayerProxy] IPlayerProxy::SetPause %d", isP);
    mux.lock();
    if (player) player->SetPause(isP);
    mux.unlock();
}

bool IPlayerProxy::IsPause() {
    XLOGI("[IPlayerProxy] IPlayerProxy::IsPause");
    bool re = false;
    mux.lock();
    if (player) re = player->IsPause();
    mux.unlock();
    return false;
}

double IPlayerProxy::PlayPos() {
    XLOGI("[IPlayerProxy] IPlayerProxy::IsPause");
    double pos = 0.0;
    mux.lock();
    if (player) pos = player->PlayPos();
    mux.unlock();
    return pos;
}

void IPlayerProxy::Init(void *vm) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Init");
    mux.lock();
    if (vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    if (!player) {
        XLOGI("IPlayerProxy::Init FFPlayerBuilder::Get()->BuilderPlayer()");
        player = FFPlayerBuilder::Get()->BuilderPlayer();
        XLOGI("IPlayerProxy::Init player=%p", &player);
    } else {
        XLOGI("IPlayerProxy::Init player not null!");
    }
    mux.unlock();
}
