//
// Created by 赵健 on 2021/9/30.
//

#ifndef NATIVECPP_IOBSERVER_H
#define NATIVECPP_IOBSERVER_H


#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>

class IObserver : public XThread{
public:
    //观察者接收数据函数
    virtual void Update(XData data){}
    // 主体函数 添加观察者（线程安全）
    void addObs(IObserver *obs);
    //通知所有观察者（线程安全）
    void Notify(XData data);

protected:
    std::vector<IObserver *>obss;
    std::mutex mux;
};


#endif //NATIVECPP_IOBSERVER_H
