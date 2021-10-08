//
// Created by 赵健 on 2021/10/1.
//

#include "IDemux.h"
#include "../XLog.h"

void IDemux::Main() {
    while (!isExit) {
        if(IsPause()) {
            XSleep(2);
            continue;
        }
        XData d = Read();
        if(d.size > 0) {
            Notify(d);
        } else {
            XSleep(2);
        }
    }
}

void IDemux::XSleep(int mis) {

}
