//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_FFRESAMPLE_H
#define NATIVECPP_FFRESAMPLE_H

#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter());

    virtual void Close();

    virtual XData Resample(XData indata);

protected:
    SwrContext *actx = 0;
    std::mutex mux;

};

#endif //NATIVECPP_FFRESAMPLE_H
