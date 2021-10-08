//
// Created by 赵健 on 2021/9/30.
//

#ifndef NATIVECPP_XDATA_H
#define NATIVECPP_XDATA_H


enum XDataType
{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

class XData {
public:
    int type = 0;
    int pts = 0;
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
    int format = 0;
    bool Alloc(int _size, const char *data = 0);
    void Drop();
};


#endif //NATIVECPP_XDATA_H
