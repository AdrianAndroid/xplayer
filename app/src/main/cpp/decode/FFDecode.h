//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_FFDECODE_H
#define NATIVECPP_FFDECODE_H

#include "../XParameter.h"
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode {
public:
    static void InitHard(void *vm);

    virtual bool Open(XParameter para, bool isHard = false);

    virtual void Close();

    virtual void Clear();

    // future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt);

    // 从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame();

    void Update(XData pkt) override;

    bool Start() override;

    void Stop() override;

    void setPause(bool isP) override;

    bool IsPause() override;

protected:
    void Main() override;

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mux;
};


#endif //NATIVECPP_FFDECODE_H
