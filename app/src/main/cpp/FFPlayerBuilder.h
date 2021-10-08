//
// Created by 赵健 on 2021/9/29.
//

#ifndef NATIVECPP_FFPLAYERBUILDER_H
#define NATIVECPP_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder : public IPlayerBuilder {
public:
    static void InitHard(void *vm);

    static FFPlayerBuilder *Get() {
        static FFPlayerBuilder ff;
        return &ff;
    }

protected:
    FFPlayerBuilder() {}

    virtual IPlayer *CreatePlayer(unsigned char index);

    virtual IDemux *CreateDemux();
    virtual IDecode *CreateDecode();
    virtual IResample *CreateResample();
    virtual IVideoView *CreateVideoView() ;
    virtual IAudioPlay *CreateAudioPlay();
};


#endif //NATIVECPP_FFPLAYERBUILDER_H
