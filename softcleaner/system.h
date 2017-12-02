#ifndef SYSTEM_H
#define SYSTEM_H

// 系统垃圾清理

#include "../cleaner.h"

class QSystemCleaner : public QCleaner
{
public:
    virtual bool Init()
    {
        this->description = "系统垃圾";
        this->removesize = 0;
        this->totalsize = 0;
        this->inited = true;
        this->mask = 0xfffffff;
        return true;
    }

    virtual void Analysis()
    {
#if defined(Q_OS_WIN)
        if (this->mask & SYS_CLEAN_SHADER)
        {
            qDebug() << "---------------------WinSAT Shader缓存---------------------";
            QVector<QString> pathvec;
            QString root = Utils::GetStandardPath("windir") + "\\Performance\\WinSAT";
            Utils::ListFiles(root, pathvec, "", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "WinSAT Shader缓存", true, SYS_CLEAN_SHADER));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_CACHE)
        {
            qDebug() << "---------------------缩略图缓存---------------------";
            QVector<QString> pathvec;
            QString root = Utils::GetStandardPath("GenericDataLocation") + "\\Microsoft\\Windows\\Explorer";
            for (const QString& rootl : Utils::GetPathForAllUsers(root))
            {
                Utils::ListFiles(rootl, pathvec, ".*\\.((db)|(tmp))", 2);
            }
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "缩略图缓存", true, SYS_CLEAN_CACHE));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_TEMP)
        {
            qDebug() << "---------------------应用缓存---------------------";
            QVector<QString> pathvec;
            QString root1 = Utils::GetStandardPath("windir") + "\\Temp";
            QString root2 = Utils::GetStandardPath("GenericDataLocation") + "\\Temp";
            Utils::ListFiles(root1, pathvec, "", 10);
            Utils::ListFiles(root2, pathvec, "", 10);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "应用缓存", true, SYS_CLEAN_TEMP));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_LOG)
        {
            qDebug() << "---------------------Windows日志---------------------";
            QVector<QString> pathvec;
            QString root1 = Utils::GetStandardPath("windir");
            QString root2 = Utils::GetStandardPath("windir") + "\\System32\\wbem\\Logs";
            QString root3 = Utils::GetStandardPath("windir") + "\\SysWOW64\\wbem\\Logs";
            QString root4 = Utils::GetStandardPath("windir") + "\\SoftwareDistribution";
            QString root5 = Utils::GetStandardPath("windir") + "\\Logs";
            QString root6 = Utils::GetStandardPath("windir") + "\\ServiceProfiles\\LocalService\\AppData";
            QString root7 = Utils::GetStandardPath("windir") + "\\ServiceProfiles\\NetworkService\\AppData";
            QString root8 = Utils::GetStandardPath("windir") + "\\security\\logs";
            QString root9 = Utils::GetStandardPath("windir") + "\\debug";
            QString root10 = Utils::GetStandardPath("GenericDataLocation") + "\\Microsoft";
            QString root11 = Utils::GetStandardPath("windir") + "\\System32\\config\\systemprofile\\AppData\\Local";
            QString root12 = Utils::GetStandardPath("windir") + "\\SysWOW64\\config\\systemprofile\\AppData\\Local";
            QString root13 = Utils::GetStandardPath("windir") + "\\System32\\WDI";
            QString root14 = Utils::GetStandardPath("windir") + "\\SysWOW64\\WDI";
            QString root15 = Utils::GetStandardPath("GenericDataLocation") + "\\Diagnostics";
            Utils::ListFiles(root1, pathvec, ".*\\.((log)|(bak)|(tmp))", 3);
            Utils::ListFiles(root2, pathvec, "", 2);
            Utils::ListFiles(root3, pathvec, "", 2);
            Utils::ListFiles(root4, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root5, pathvec, "", 2);
            Utils::ListFiles(root6, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root7, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root8, pathvec, "", 2);
            Utils::ListFiles(root9, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root10, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root11, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root12, pathvec, ".*\\.((log)|(bak)|(tmp))", 2);
            Utils::ListFiles(root13, pathvec, "", 2);
            Utils::ListFiles(root14, pathvec, "", 2);
            Utils::ListFiles(root15, pathvec, "", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows日志", true, SYS_CLEAN_LOG));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_WER)
        {
            qDebug() << "---------------------Windows错误报告---------------------";
            QVector<QString> pathvec;
            QString root1 = Utils::GetStandardPath("GenericDataLocation") + "\\Microsoft\\Windows\\WER\\ReportArchive";
            QString root2 = Utils::GetStandardPath("programdata") + "\\Microsoft\\Windows\\WER\\ReportArchive";
            Utils::ListFiles(root1, pathvec, "", 2);
            Utils::ListFiles(root2, pathvec, "", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows日志", true, SYS_CLEAN_WER));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_DOWNLOAD)
        {
            qDebug() << "---------------------Windows下载目录---------------------";
            QVector<QString> pathvec;
            QString root = Utils::GetStandardPath("HomeLocation") + "\\Downloads";
            for (const QString& rootl : Utils::GetPathForAllUsers(root))
            {
                Utils::ListFiles(rootl, pathvec, "", 10);
            }
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows下载目录", true, SYS_CLEAN_DOWNLOAD));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_BAK)
        {
            qDebug() << "---------------------Windows备份文件---------------------";
            QVector<QString> pathvec;
            for (const QString& p : Utils::GetPartitions())
            {
                Utils::ListFiles(p + "\\Windows.old", pathvec, "", 100);
                Utils::ListFiles(p + "\\$Windows.~BT", pathvec, "", 100);
            }
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows备份文件", true, SYS_CLEAN_BAK));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_RECYCLE)
        {
            qDebug() << "---------------------Windows回收站---------------------";
            QVector<QString> pathvec;
            for (const QString& p : Utils::GetPartitions())
            {
                Utils::ListFiles(p + "\\Recycler", pathvec, "", 100);
                Utils::ListFiles(p + "\\Recycled", pathvec, "", 100);
                Utils::ListFiles(p + "\\$RECYCLE.BIN", pathvec, "", 100);
            }
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows备份文件", true, SYS_CLEAN_RECYCLE));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_PREFETCH)
        {
            qDebug() << "---------------------Windows预读数据---------------------";
            QVector<QString> pathvec;
            QString root = Utils::GetStandardPath("windir") + "\\Prefetch";
            Utils::ListFiles(root, pathvec, "", 10);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows下载目录", true, SYS_CLEAN_PREFETCH));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_DRIVER)
        {
            qDebug() << "---------------------Windows驱动---------------------";
            QVector<QString> pathvec;
            QString root1 = Utils::GetStandardPath("windir") + "\\System32\\DriverStore\\FileRepository";
            QString root2 = Utils::GetStandardPath("windir") + "\\SysWOW64\\DriverStore\\FileRepository";
            Utils::ListFiles(root1, pathvec, ".*((prn)|(mdm)).*", 2);
            Utils::ListFiles(root2, pathvec, ".*((prn)|(mdm)).*", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows驱动", true, SYS_CLEAN_DRIVER));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_UPDATE)
        {
            qDebug() << "---------------------Windows升级文件缓存---------------------";
            QVector<QString> pathvec;
            QString root = Utils::GetStandardPath("windir") + "\\SoftwareDistribution\\Download";
            Utils::ListFiles(root, pathvec, "", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows升级文件缓存", true, SYS_CLEAN_UPDATE));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
        if (this->mask & SYS_CLEAN_FONT)
        {
            qDebug() << "---------------------Windows字体缓存---------------------";
            QVector<QString> pathvec;
            QString root1 = Utils::GetStandardPath("windir") + "\\ServiceProfiles\\LocalService\\AppData\\Local";
            QString root2 = Utils::GetStandardPath("GenericDataLocation");
            Utils::ListFiles(root1, pathvec, ".*FontCache.*", 2);
            Utils::ListFiles(root2, pathvec, ".*gdipfontcache.*", 2);
            for (const QString& p : pathvec)
            {
                qDebug () << p;
                this->targets.push_back(QCleanFileInfo(p, "Windows字体缓存", true, SYS_CLEAN_FONT));
                this->totalsize += Utils::GetFileSize(p);
            }
        }
#endif
    }
};

#endif // SYSTEM_H
