//
// Created by 赵健 on 2021/9/29.
//

#ifndef NATIVECPP_IPLAYERBUILDER_H
#define NATIVECPP_IPLAYERBUILDER_H

#include "IPlayer.h"

class IPlayerBuilder {
public:
    virtual IPlayer *BuilderPlayer(unsigned char index=0);
protected:
//    virtual IPlayer *CreatePlayer(unsigned char index=0) = 0;
    virtual IDemux *CreateDemux() = 0;
    virtual IDecode *CreateDecode() = 0;
    virtual IResample *CreateResample() = 0;
    virtual IVideoView *CreateVideoView()  = 0;
    virtual IAudioPlay *CreateAudioPlay() = 0;
    virtual IPlayer *CreatePlayer(unsigned char index=0) = 0;
};


#endif //NATIVECPP_IPLAYERBUILDER_H
