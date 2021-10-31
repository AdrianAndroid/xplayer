//
// Created by 赵健 on 2021/9/30.
//

#include "IObserver.h"
#include "XLog.h"

// 主体函数 添加观察者
void IObserver::addObs(IObserver *obs) {
    if(!obs) return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

// 同时所有观察者
void IObserver::Notify(XData data) {
    mux.lock();
    //XLOGI("IObserver::Notify obs.size = %d", obss.size());
    for (int i = 0; i < obss.size(); i++) {
        obss[i]->Update(data);
    }
    mux.unlock();
}
