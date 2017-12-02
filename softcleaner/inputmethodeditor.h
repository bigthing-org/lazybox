#ifndef INPUTMETHODEDITOR_H
#define INPUTMETHODEDITOR_H

#include "../cleaner.h"

// 输入法

class QBaiduImeCleaner : QCleaner
{
public:
    virtual bool Init() { QCleaner::Init(); return true; }

    virtual void Analysis()
    {
        QString baiduimedir = GetBaiduImeDir();
        if (baiduimedir.isEmpty())
        {
            return;
        }
        QVector<QString> pathset;
        Utils::ListFiles(baiduimedir + "\\Account", pathset, "", 10);
        Utils::ListFiles(baiduimedir + "\\AutoUpdate", pathset, "", 10);
        Utils::ListFiles(baiduimedir + "\\CacheData", pathset, "", 10);
        Utils::ListFiles(baiduimedir + "\\LogCache", pathset, "", 10);
        Utils::ListFiles(baiduimedir + "\\Face", pathset, "", 10);
        Utils::ListFiles(baiduimedir + "\\Temp", pathset, "", 10);

        qDebug() << "--------------------------百度输入法清理---------------------------";
        for (const QString& p : pathset)
        {
            qDebug() << p;
            this->targets.push_back(QCleanFileInfo(p, "百度输入法文件", true, IME_CLEAN_BAIDUIME));
            this->totalsize += Utils::GetFileSize(p);
        }
    }

    virtual void Delete()
    {

    }

    virtual QString GetBaiduImeDir()
    {
        QString path = Utils::GetStandardPath("GenericDataLocation") + "\\Baidu\\BaiduPinyin";
        if (Utils::FileExist(path))
        {
            return path;
        }
        path = path.replace("Local", "LocalLow");
        if (Utils::FileExist(path))
        {
            return path;
        }
        return "";
    }
};

#endif // INPUTMETHODEDITOR_H
