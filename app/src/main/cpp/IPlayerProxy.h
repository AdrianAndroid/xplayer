//
// Created by 赵健 on 2021/9/27.
//

#ifndef NATIVECPP_IPLAYERPROXY_H
#define NATIVECPP_IPLAYERPROXY_H

#include "IPlayer.h"
#include <mutex>

class IPlayerProxy : public IPlayer {
public:
    // 静态全局
    static IPlayerProxy *Get() {
        static IPlayerProxy px;
        return &px;
    }
    void Init(void *vm = 0);

    virtual bool Open(const char *path);
    virtual bool Seek(double pos);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void SetPause(bool isP);
    virtual bool IsPause();
    //获取当前播放进度 0.0 ~ 1.0
    virtual double PlayPos();

protected:
    IPlayerProxy() {}

    IPlayer *player = 0;
    std::mutex mux;
};


#endif //NATIVECPP_IPLAYERPROXY_H
