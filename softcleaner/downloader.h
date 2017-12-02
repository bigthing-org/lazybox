#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "../cleaner.h"

// 下载工具清理

// 迅雷看看清理
class QThunderCleaner : QCleaner
{
public:
    virtual bool Init() { QCleaner::Init(); return true; }

    virtual void Analysis()
    {
        QString thunderdir = GetThunderDir();
        if (thunderdir.isEmpty())
        {
            return;
        }
        QStringList pathset;

        // 下载文件
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(thunderdir + "\\Profiles\\TaskDb.dat");
        if (db.open())
        {
            QSqlQuery query;
            query.prepare("select SavePath, Name from TaskBase");
            if (query.exec())
            {
                while (query.next())
                {
                    QString savepath = query.value("SavePath").toString().toUtf8().data();
                    QString name = query.value("Name").toString().toUtf8().data();
                    pathset.push_back(savepath + name);
                }
            }
            db.close();
        }

        qDebug() << "--------------------------迅雷文件---------------------------";
        for (const QString& p : pathset)
        {
            qDebug() << p;
            this->targets.push_back(QCleanFileInfo(p, "迅雷文件", true, DOWNLOAD_CLEAN_THUNDER));
            this->totalsize += Utils::GetFileSize(p);
        }
    }

    virtual void Delete()
    {
        // 清空TaskDb.dat
        // 清空cid_store.dat
        // 删除下载文件
        // 删除临时文件
    }

    virtual QString GetThunderDir() // 获取所有可找到的迅雷安装目录
    {
        // 搜索本地路径
        for (QString& drive : Utils::GetPartitions())
        {
            QString suffix[] =
            {
                "\\Program Files\\Thunder Network\\Thunder",
                "\\Program Files\\Thunder Network\\Thunder9",
                "\\Program Files (x86)\\Thunder Network\\Thunder",
                "\\Program Files (x86)\\Thunder Network\\Thunder9"
            };
            for (int i = 0; i < ARRLEN(suffix); i++)
            {
                QString path = drive + suffix[i];
                if (Utils::FileExist(path))
                {
                    return path;
                }
            }
        }
        // 搜索注册表
        QSettings reg("HKEY_CLASSES_ROOT\\thunder\\DefaultIcon", QSettings::NativeFormat);
        if (reg.contains("."))
        {
            QString thunderpath = reg.value(".").toString();
            QString path = thunderpath.left(thunderpath.indexOf("\\Program\\Thunder.exe"));
            if (Utils::FileExist(path))
            {
                return path;
            }
        }
        return "";
    }
};

// 百度网盘清理
class QBaiduNetDiskCleaner : QCleaner
{
public:
    virtual bool Init() { QCleaner::Init(); return true; }

    virtual void Analysis()
    {
        QString baidunetdiskdir = GetBaiduNetDiskDir();
        if (baidunetdiskdir.isEmpty())
        {
            return;
        }
        QStringList pathset;
        for (const QString& subdir : Utils::GetDownDirs(baidunetdiskdir + "\\users"))
        {// ...\Baidu\BaiduNetdisk\users\83318853b0d5e4b06dae0f32be0245e3\BaiduYunGuanjia.db
            QString dbfile = subdir + "\\BaiduYunGuanjia.db";
            if (Utils::FileExist(dbfile))
            {
                // 下载文件
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(dbfile);
                if (db.open())
                {
                    QSqlQuery query;
                    query.prepare("select local_filename, base_dir from backup_file");
                    if (query.exec())
                    {
                        while (query.next())
                        {
                            QString path = query.value("SavePath").toString();
                            QString file = query.value("Name").toString();
                            pathset.push_back(path + file);
                        }
                    }
                    query.prepare("select local_path from download_file");
                    if (query.exec())
                    {
                        while (query.next())
                        {
                            QString path = query.value("local_path").toString();
                            pathset.push_back(path);
                        }
                    }
                    query.prepare("select local_path from download_history_file");
                    if (query.exec())
                    {
                        while (query.next())
                        {
                            QString path = query.value("local_path").toString();
                            pathset.push_back(path);
                        }
                    }
                    query.prepare("select local_path from upload_file");
                    if (query.exec())
                    {
                        while (query.next())
                        {
                            QString path = query.value("local_path").toString();
                            pathset.push_back(path);
                        }
                    }
                    query.prepare("select local_path from upload_history_file");
                    if (query.exec())
                    {
                        while (query.next())
                        {
                            QString path = query.value("local_path").toString();
                            pathset.push_back(path);
                        }
                    }

                    db.close();
                }
            }
        }
        qDebug() << "----------------------百度网盘文件----------------------";
        for (const QString& p : pathset)
        {
            qDebug() << p;
            this->targets.push_back(QCleanFileInfo(p, "百度网盘文件", true, DOWNLOAD_CLEAN_BAIDUDISK));
            this->totalsize += Utils::GetFileSize(p);
        }
    }

    virtual void Delete()
    {
        // BaiduYunGuanjia.db
        // 删除下载文件
        // 删除临时文件
    }

    virtual QString GetBaiduNetDiskDir()
    {
        // 搜索注册表
        {
            QSettings reg("HKEY_CURRENT_USER\\Software\\Baidu\\BaiduYunGuanjia", QSettings::NativeFormat);
            if (reg.contains("installDir"))
            {
                QString baidupath = reg.value("installDir").toString();
                QString path = baidupath.left(baidupath.indexOf("\\Program\\Thunder.exe"));
                if (Utils::FileExist(path))
                {
                    return path;
                }
            }
        }

        {
            QSettings reg("HKEY_CURRENT_USER\\Software\\Wow6432Node\\Baidu\\BaiduYunGuanjia", QSettings::NativeFormat);
            if (reg.contains("installDir"))
            {
                QString baidupath = reg.value("installDir").toString();
                QString path = baidupath.left(baidupath.indexOf("\\Program\\Thunder.exe"));
                if (Utils::FileExist(path))
                {
                    return path;
                }
            }
        }

        {
            QSettings reg("HKEY_LOCAL_MACHINE\\Software\\Baidu\\BaiduYunGuanjia", QSettings::NativeFormat);
            if (reg.contains("installDir"))
            {
                QString baidupath = reg.value("installDir").toString();
                QString path = baidupath.left(baidupath.indexOf("\\Program\\Thunder.exe"));
                if (Utils::FileExist(path))
                {
                    return path;
                }
            }
        }

        {
            QSettings reg("HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\Baidu\\BaiduYunGuanjia", QSettings::NativeFormat);
            if (reg.contains("installDir"))
            {
                QString baidupath = reg.value("installDir").toString();
                QString path = baidupath.left(baidupath.indexOf("\\Program\\Thunder.exe"));
                if (Utils::FileExist(path))
                {
                    return path;
                }
            }
        }
        return "";
    }
};

#endif // DOWNLOADER_H
