//
// Created by 赵健 on 2021/9/27.
//

#include "IPlayer.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    return false;
}

void IPlayer::Close() {

}

bool IPlayer::Start() {
    return false;
}

void IPlayer::InitView(void *win) {

}

double IPlayer::PlayPos() {
    return 0;
}

bool IPlayer::Seek(double pos) {
    return false;
}

void IPlayer::SetPause(bool isP) {

}
