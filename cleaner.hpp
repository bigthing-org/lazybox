#ifndef CLEANER_H
#define CLEANER_H

#include "commonheader.h"
#include "plugin.h"
#include "utils.hpp"

// 该文件实现清理模块

enum
{
    SYS_CLEAN = 1, /////////////////////////
    SYS_CLEAN_SHADER = 1,
    SYS_CLEAN_CACHE = 2,
    SYS_CLEAN_TEMP = 4,
    SYS_CLEAN_LOG = 8,
    SYS_CLEAN_WER = 0x10,
    SYS_CLEAN_DOWNLOAD = 0x20,
    SYS_CLEAN_BAK = 0x40,
    SYS_CLEAN_RECYCLE = 0x80,
    SYS_CLEAN_PREFETCH = 0x100,
    SYS_CLEAN_DRIVER = 0x200,
    SYS_CLEAN_UPDATE = 0x400,
    SYS_CLEAN_FONT = 0x800,

    DOWNLOAD_CLEAN = 2, ////////////////////
    DOWNLOAD_CLEAN_THUNDER = 1,
    DOWNLOAD_CLEAN_BAIDUDISK = 2,

    PLAYER_CLEAN = 4, /////////////////////
    PLAYER_CLEAN_XMP = 1,

    IME_CLEAN = 8, ////////////////////////
    IME_CLEAN_BAIDUIME = 1,
};

// 记录文件信息
class QCleanFileInfo
{
public:
    int filetype;       // 文件/目录?
    int cleantype;      // 清理类型
    bool deletef;       // 是否默认删除
    QString path;       // 文件路径
    QString info;       // 文件信息

public:
    QCleanFileInfo() {}

    QCleanFileInfo(const QString& path, const QString& info, bool deletef, int cleantype)
    {
        this->filetype = 0;
        if (Utils::IsFile(path))
        {
            this->filetype = QDir::Files;
        }
        else if (Utils::IsFile(path))
        {
            this->filetype = QDir::Dirs;
        }
        this->path = path;
        this->info = info;
        this->deletef = deletef;
        this->cleantype = cleantype;
    }

    QCleanFileInfo(const QCleanFileInfo& c)
    {
        this->path = c.path;
        this->info = c.info;
        this->deletef = c.deletef;
        this->cleantype = c.cleantype;
    }
};

// 清理者基类
class QCleaner : public LazyBoxPlugin
{
public:
    QCleaner() { }

    // 安装插件
    virtual bool Init() { return true; }

    // 卸载插件
    virtual void Uninit() {}

    // 插件id
    virtual void GetPluginId() {}

    // 实现该类型扫描
    virtual void Analysis() = 0;

    // 实现删除
    virtual void Delete() { }

protected:
    QString description;// 说明
    qint64 totalsize;// 总空间大小
    qint64 removesize;// 成功清理的的空间大小
    QVector<QCleanFileInfo> targets;
    bool inited;
    int mask;

protected:
};

#endif // CLEANER_H
