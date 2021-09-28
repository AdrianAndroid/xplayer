//
// Created by 赵健 on 2021/9/27.
//

#ifndef NATIVECPP_IPLAYERPROXY_H
#define NATIVECPP_IPLAYERPROXY_H

#include "IPlayer.h"

class IPlayerProxy : public IPlayer {
    // 静态全局
    static IPlayerProxy *Get() {
        static IPlayerProxy px;
        return &px;
    }

protected:
    IPlayerProxy() {}

    IPlayer *player = 0;

};


#endif //NATIVECPP_IPLAYERPROXY_H
