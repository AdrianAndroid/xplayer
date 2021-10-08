//
// Created by 赵健 on 2021/10/1.
//

#ifndef NATIVECPP_XTHREAD_H
#define NATIVECPP_XTHREAD_H

// sleep毫秒
void XSleep(int mis);

class XThread {
public:
    // 启动线程
    virtual bool Start();
    // 通过控制isExit安全停止线程（不一定成功）
    virtual void Stop();
    virtual void setPause(bool isP);
    virtual bool IsPause() {
        isPausing = isPause;
        return isPause;
    }
    // 入口主函数
    virtual void Main() {}
protected:
    bool isExit = false;
    bool isRunning = false;
    bool isPause = false;
    bool isPausing = false;

private:
    void ThreadMain();
};


#endif //NATIVECPP_XTHREAD_H
