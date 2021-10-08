//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_IAUDIOPLAY_H
#define NATIVECPP_IAUDIOPLAY_H

#include <list>
#include "../IObserver.h"
#include "../XParameter.h"

class IAudioPlay : public IObserver {
public:
//    // 缓冲满后阻塞
    virtual  void Update(XData data) = 0;
//    // 获取换从数据，如没有则阻塞
    virtual XData GetData() = 0;
    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear() = 0;

    // 最大缓冲
    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //NATIVECPP_IAUDIOPLAY_H
