//
// Created by 赵健 on 2021/10/1.
//

#ifndef NATIVECPP_XPARAMETER_H
#define NATIVECPP_XPARAMETER_H

struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;
};


#endif //NATIVECPP_XPARAMETER_H
