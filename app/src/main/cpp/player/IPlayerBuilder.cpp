//
// Created by 赵健 on 2021/9/29.
//

#include "IPlayerBuilder.h"
#include "../XLog.h"

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
    if (de) {
        XLOGI("IPlayerBuilder::BuilderPlayer demux create succeed!");
        play->demux = de;
    } else {
        XLOGI("IPlayerBuilder::BuilderPlayer demux create failed!");
    }


    return play;
}