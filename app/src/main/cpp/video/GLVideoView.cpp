//
// Created by 赵健 on 2021/10/8.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "../XLog.h"


void GLVideoView::SetRender(void *win)
{
    view = win;
}
void GLVideoView::Close()
{
    mux.lock();
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }

    mux.unlock();
}
void GLVideoView::Render(XData data)
{
    XLOGI("GLVideoView::Render");
    if(!view) return;
    if(!txt)
    {
        txt = XTexture::Create();

        txt->Init(view,(XTextureType)data.format);
    }
    txt->Draw(data.datas,data.width,data.height);
}