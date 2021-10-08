//
// Created by 赵健 on 2021/10/8.
//

#include "IVideoView.h"

void IVideoView::Update(XData data) {
    //IObserver::Update(data);
    this->Render(data);
}
