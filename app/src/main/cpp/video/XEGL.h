//
// Created by 赵健 on 2021/10/13.
//

#ifndef NATIVECPP_XEGL_H
#define NATIVECPP_XEGL_H


class XEGL
{
public:
    virtual bool Init(void *win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    static XEGL *Get();

protected:
    XEGL(){}
};



#endif //NATIVECPP_XEGL_H
