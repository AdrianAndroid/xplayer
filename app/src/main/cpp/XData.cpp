//
// Created by 赵健 on 2021/9/30.
//

#include "XData.h"

extern "C" {
#include <libavformat/avformat.h>
}

bool XData::Alloc(int _size, const char *d) {
    Drop();
    type = UCHAR_TYPE;
    if (_size <= 0) return false;
    this->data = new unsigned char[_size];
    if (!this->data) return false;
    if (d) {
        memcpy(this->data, d, _size);
    }
    this->size = _size;
    return true;
}

void XData::Drop() {
    if (!data) return;
    if (type == AVPACKET_TYPE) {
        av_packet_free((AVPacket **) &data);
    } else {
        delete data;
    }
    data = nullptr;
    size = 0;
}
