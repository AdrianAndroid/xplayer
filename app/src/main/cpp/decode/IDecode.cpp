//
// Created by 赵健 on 2021/10/8.
//

#include "IDecode.h"
#include "../XLog.h"

void IDecode::Clear() {
    packsMutex.lock();
    while (!packs.empty()) {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
}

void IDecode::Update(XData pkt) {
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        packsMutex.lock();
        // 阻塞
        if (packs.size() < maxList) {
            //生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }
}

void IDecode::Main() {
    while (!isExit) {
        if (IsPause()) {
            XSleep(2);
            continue;
        }
        packsMutex.lock();

        // 判断音视频同步
//        if (!isAudio && synPts > 0) {
//            if (synPts < pts) {
//                packsMutex.unlock();
//                XSleep(1);
//                continue;
//            }
//        }

        if (packs.empty()) {
            packsMutex.unlock();
            XSleep(1);
            //XLOGI("[IDecode::Main()] packs.empty()");
            continue;
        } else {
            //XLOGI("[IDecode::Main()] packs not empty");
        }

        // 取出packet消费者
        XData pack = packs.front();
        packs.pop_front();

        //发送数据到解码线程， 一个数据包 可能解码多个结果
        if (this->SendPacket(pack)) {
            //XLOGI("[IDecode::Main()] this->SendPacket(pack) begin!");
            while (!isExit) {
                // 获取解码数据
                XData frame = RecvFrame();
                if (!frame.data) {
                    //XLOGI("[IDecode::Main()] !frame.data");
                    break;
                } else {
                    //XLOGI("[IDecode::Main()] frame.data");
                }
                pts = frame.size;
                //发送数据给观察者
                //XLOGI("[IDecode::Main()] this->Notify(frame)");
                this->Notify(frame);
            }
            //XLOGI("[IDecode::Main()] this->SendPacket(pack) done!");
        } else {
            XLOGI("[IDecode::Main()] this->SendPacket(pack) false!");
        }
        pack.Drop();
        packsMutex.unlock();
    }
}
