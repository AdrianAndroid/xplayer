//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_IRESAMPLE_H
#define NATIVECPP_IRESAMPLE_H


#include "../IObserver.h"
#include "../XParameter.h"

class IResample : public IObserver {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;

    virtual XData Resample(XData indata) = 0;

    virtual void Close() = 0;

    virtual void Update(XData data);

    int outChannels = 2;
    int outFormat = 1;
};


#endif //NATIVECPP_IRESAMPLE_H
