//
// Created by 赵健 on 2021/9/27.
//

#ifndef NATIVECPP_IPLAYER_H
#define NATIVECPP_IPLAYER_H


class IPlayer {
public:
    static IPlayer *Get(unsigned char index = 0);

    virtual bool Open(const char *path);
    virtual void Close();

    virtual bool Start();
    virtual void InitView(void *win);

    // 获取当前播放进度 0.0 ~ 1.0
    virtual double PlayPos();
    virtual bool  Seek(double pos);
    virtual void SetPause(bool  isP);

    //是否视频硬解码
    bool isHardDecode = true;

    //音频输出参数配置
};


#endif //NATIVECPP_IPLAYER_H
