//
// Created by 赵健 on 2021/9/27.
//

#ifndef NATIVECPP_IPLAYER_H
#define NATIVECPP_IPLAYER_H
#include <mutex>
#include "../XThread.h"
#include "../XParameter.h"


class IDemux;
class IAudioPlay;
class IVideoView;
class IResample;
class IDecode;

class IPlayer : public XThread{
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
    bool isHardDecode = false;

    //音频输出参数配置
    XParameter outPara;

    IDemux *demux = 0; // 解码视频数据
    IDecode *vdecode = 0; // 视频解码
    IDecode *adecode = 0; // 音频解码
    IResample *resample = 0; // 重采样
    IVideoView *videoView = 0; // 显示的界面
    IAudioPlay *audioPlay = 0; // 播放的声音

protected:
    // 用作音视频同步
    void Main();
    std::mutex mux;
    IPlayer(){};
};


#endif //NATIVECPP_IPLAYER_H
