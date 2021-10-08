//
// Created by 赵健 on 2021/9/29.
//

#include "IPlayerBuilder.h"
#include "../XLog.h"
#include "../video/IVideoView.h"
#include "../resample/IResample.h"
#include "../audio/IAudioPlay.h"
#include "../demux/IDemux.h"
#include "../decode/IDecode.h"

void printMessage(bool suc, const char *msg) {
    if (suc) {
        XLOGI("IPlayerBuilder::BuilderPlayer %s create succeed!", msg);
    } else {
        XLOGE("IPlayerBuilder::BuilderPlayer %s create failed!", msg);
    }
}

IPlayer *IPlayerBuilder::BuilderPlayer(unsigned char index) {
    XLOGI("IPlayerBuilder::BuilderPlayer");
    IPlayer *play = CreatePlayer(index);

    if (play) {
        XLOGI("IPlayerBuilder::BuilderPlayer play create succeed!");
    } else {
        XLOGI("IPlayerBuilder::BuilderPlayer play create failed!");
        return play;
    }

    // 解封装
    IDemux *de = CreateDemux();

    // 视频解码
    IDecode *vdecode = CreateDecode();
    // 音频解码
    IDecode *adecode = CreateDecode();
    // 解码器观察解封装
    de->addObs(vdecode);
    de->addObs(adecode);

    // 看是观察视频解码器
    IVideoView *view = CreateVideoView();
    vdecode->addObs(view);

    // 重采样观察音频重采样
    IResample *resample = CreateResample();
    adecode->addObs(resample);

    //音频播放观察采样
    IAudioPlay *audioPlay = CreateAudioPlay();
    resample->addObs(audioPlay);

    printMessage(de, "demux");
    printMessage(adecode, "adecode");
    printMessage(vdecode, "vdecode");
    printMessage(view, "videoView");
    printMessage(resample, "resample");
    printMessage(audioPlay, "audioPlay");


    play->demux = de;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->audioPlay = audioPlay;
    return play;
}