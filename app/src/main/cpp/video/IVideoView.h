//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_IVIDEOVIEW_H
#define NATIVECPP_IVIDEOVIEW_H

#include "../IObserver.h"
#include "../XData.h"

class IVideoView : public IObserver {
public:
    virtual void SetRender(void *win) = 0;

    virtual void Render(XData data) = 0;

    virtual void Update(XData data);

    virtual void Close() = 0;
};


#endif //NATIVECPP_IVIDEOVIEW_H
