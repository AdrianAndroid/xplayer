//
// Created by 赵健 on 2021/9/29.
//

#include "FFPlayerBuilder.h"
#include "../demux/FFDemux.h"
#include "../decode/FFDecode.h"
#include "../video/GLVideoView.h"
#include "../audio/SLAudioPlay.h"
#include "../resample/FFResample.h"
#include "../XLog.h"

void FFPlayerBuilder::InitHard(void *vm) {
    XLOGI("FFPlayerBuilder::InitHard(void *vm)");
}

// 播放器句柄
IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index) {
    XLOGI("IPlayer *FFPlayerBuilder::CreatePlayer index=%d", index);
    return IPlayer::Get(index);
}

// 解析文件
IDemux *FFPlayerBuilder::CreateDemux() {
    XLOGI("IDemux *FFPlayerBuilder::CreateDemux()");
    IDemux *ff = new FFDemux();
    return ff;
}

// 解码器
IDecode *FFPlayerBuilder::CreateDecode() {
    IDecode *ff = new FFDecode();
    return ff;
}

// 重采样
IResample *FFPlayerBuilder::CreateResample() {
    IResample *ff = new FFResample();
    return ff;
}

// 播放视频
IVideoView *FFPlayerBuilder::CreateVideoView() {
    IVideoView *ff = new GLVideoView();
    return ff;
}

// 播放音频
IAudioPlay *FFPlayerBuilder::CreateAudioPlay() {
    IAudioPlay *ff = new SLAudioPlay();
    return ff;
}
