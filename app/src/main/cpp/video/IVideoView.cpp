//
// Created by 赵健 on 2021/10/8.
//

#include "IVideoView.h"
#include "../XLog.h"

void IVideoView::Update(XData data) {
    //IObserver::Update(data);
    //XLOGI("[IVideoView::Update]");
    this->Render(data);
}
