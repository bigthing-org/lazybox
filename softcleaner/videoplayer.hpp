#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H


#include "../cleaner.hpp"

// 视频音频图片播放

// 迅雷看看清理
class QXMPCleaner : QCleaner
{
public:
    virtual bool Init() { QCleaner::Init(); return true; }

    virtual void Analysis()
    {


    }

    virtual void Delete()
    {
    }

    virtual QString GetXMPDir()
    {

    }
};

#endif // VIDEOPLAYER_H
