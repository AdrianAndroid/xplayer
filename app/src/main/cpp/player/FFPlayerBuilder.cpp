//
// Created by 赵健 on 2021/9/29.
//

#include "FFPlayerBuilder.h"
#include "../demux/FFDemux.h"
#include "../XLog.h"

void FFPlayerBuilder::InitHard(void *vm) {
    XLOGI("FFPlayerBuilder::InitHard(void *vm)");
}

IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index) {
    XLOGI("IPlayer *FFPlayerBuilder::CreatePlayer index=%d", index);
    return IPlayer::Get(index);
}

IDemux *FFPlayerBuilder::CreateDemux() {
    XLOGI("IDemux *FFPlayerBuilder::CreateDemux()");
    IDemux *ff = new FFDemux();
    return ff;
}

IDecode *FFPlayerBuilder::CreateDecode() {
    return nullptr;
}

IResample *FFPlayerBuilder::CreateResample() {
    return nullptr;
}

IVideoView *FFPlayerBuilder::CreateVideoView() {
    return nullptr;
}

IAudioPlay *FFPlayerBuilder::CreateAudioPlay() {
    return nullptr;
}