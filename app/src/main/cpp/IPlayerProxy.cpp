//
// Created by 赵健 on 2021/9/27.
//

#include "IPlayerProxy.h"
#include "XLog.h"

bool IPlayerProxy::Open(const char *path) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Open %s", path);
    return IPlayer::Open(path);
}

bool IPlayerProxy::Seek(double pos) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Seek %f", pos);
    return IPlayer::Seek(pos);
}

void IPlayerProxy::Close() {
    XLOGI("[IPlayerProxy] IPlayerProxy::Close");
    IPlayer::Close();
}

bool IPlayerProxy::Start() {
    XLOGI("[IPlayerProxy] IPlayerProxy::Start");
    return IPlayer::Start();
}

void IPlayerProxy::InitView(void *win) {
    XLOGI("[IPlayerProxy] IPlayerProxy::InitView");
    IPlayer::InitView(win);
}

void IPlayerProxy::SetPause(bool isP) {
    XLOGI("[IPlayerProxy] IPlayerProxy::SetPause %d", isP);
    IPlayer::SetPause(isP);
}

bool IPlayerProxy::IsPause() {
    XLOGI("[IPlayerProxy] IPlayerProxy::IsPause");
    return false;
}

double IPlayerProxy::PlayPos() {
    XLOGI("[IPlayerProxy] IPlayerProxy::IsPause");
    return 0;
}

void IPlayerProxy::Init(void *vm) {
    XLOGI("[IPlayerProxy] IPlayerProxy::Init");
}
