//
// Created by 赵健 on 2021/10/8.
//

#include "IResample.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    if (d.size > 0) {
        this->Notify(d);
    }
}