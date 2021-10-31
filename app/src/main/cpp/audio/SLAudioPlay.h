//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_SLAUDIOPLAY_H
#define NATIVECPP_SLAUDIOPLAY_H

#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay {
public:
    virtual bool StartPlay(XParameter out);

    virtual void Close();

    void PlayCall(void *bufq);

    SLAudioPlay();

    ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //NATIVECPP_SLAUDIOPLAY_H
