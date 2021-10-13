//
// Created by 赵健 on 2021/10/8.
//

#ifndef NATIVECPP_GLVIDEOVIEW_H
#define NATIVECPP_GLVIDEOVIEW_H

#include "../XData.h"
#include "IVideoView.h"

class XTexture;

class GLVideoView : public IVideoView{
public:
    virtual void SetRender(void *win);

    virtual void Render(XData data);

    virtual void Close();

protected:
    void *view = 0;
    XTexture *txt = 0;
    std::mutex mux;
};


#endif //NATIVECPP_GLVIDEOVIEW_H
