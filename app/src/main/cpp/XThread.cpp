//
// Created by 赵健 on 2021/10/1.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

using namespace std;
void XSleep(int mis){
    chrono::milliseconds  du(mis);
    this_thread::sleep_for(du);
}

bool XThread::Start() {
    isExit = false;
    isPausing = false;
    thread th(&XThread::ThreadMain, this);
    th.detach();
    return true;
}

void XThread::Stop() {
    XLOGI("STOP 停止线程begin！");
    for (int i = 0; i < 200; i++) {
        if(!isRunning) {
            XLOGI("Stop 停止线程成功！");
            return;
        }
        XSleep(1);
    }
    XLOGI("Stop 停止线程超时!");
}

void XThread::setPause(bool isP) {
    isPause = isP;
    // 等待100毫秒
    for (int i = 0; i < 10; i++) {
        if(isPausing == isP){
            break;
        }
        XSleep(10);
    }
}

void XThread::ThreadMain() {
    isRunning = true;
    XLOGI("线程函数进入");
    Main();
    XLOGI("线程函数退出");
    isRunning = false;
}
