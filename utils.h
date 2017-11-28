#ifndef UTILS_H
#define UTILS_H

// 常用函数

#include "commonheader.h"

class Utils
{
public:
    /*---------------------------文件操作---------------------------*/
    static bool FileExist(const QString& path) { return  QFile(path).exists(); }
    static bool DirExist(const QString& path) { return  QDir(path).exists(); }
    static long long GetFileSize(const QString& path) { return QFile(path).size(); }
    static bool IsDir(const QString& path) { return QFileInfo(path).isDir(); }
    static bool IsFile(const QString& path) { return QFileInfo(path).isFile(); }

    // 获取指定目录下所有文件
    static void ListFiles(const QString& root, QVector<QString>& out, const QString& filter="",
                          int maxlayer=1)
    {
        QDir dir(root);
        QRegExp rx(filter);
        for (const QFileInfo& fi : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
        {
            QString p = fi.absoluteFilePath();
            if (filter.isEmpty() || rx.exactMatch(filter))
            {
                out.push_back(p);
                maxlayer--;
                if (maxlayer > 0)
                {
                    ListFiles(p, out, filter, maxlayer);
                }
            }
        }
    }

    static QVector<QString> GetPartitions()
    {
        QVector<QString> result;
        for (const QFileInfo& fi : QDir::drives())
        {
            result.push_back(fi.absoluteFilePath());
        }
        return result;
    }

    static QString GetCurrentDir() { return QCoreApplication::applicationDirPath(); }

    // 获取上一级目录
    static QString GetUpDir(const QString& path) { return QFileInfo(path).absolutePath(); }

    // 获取下一级目录
    static QVector<QString> GetDownDirs(const QString& path)
    {
        QVector<QString> out;
        QDir dir(path);
        for (const QFileInfo& fi : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
        {
            if (fi.isDir())
            {
                out.push_back(fi.absoluteFilePath());
            }
        }
        return out;
    }

    /*---------------------------注册表操作---------------------------*/


    /*---------------------------账户操作---------------------------*/
    static QVector<QString> GetAllUsersNames()
    {
        QVector<QString> users;
        QString userdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        for (const QString& dir : GetDownDirs(GetUpDir(userdir)))
        {
            users.push_back(dir.section("/", -1, -1));
        }
        return users;
    }

    static QString GetCurrentUserName()
    {
        return QStandardPaths::writableLocation(QStandardPaths::HomeLocation).section("/", -1, -1);
    }


    /*---------------------------网络操作---------------------------*/
    static int GetHttpRequest(const QString& url, const QString& method, const QString& postdata, QString& response,
                              const QNetworkProxy& proxy=QNetworkProxy(QNetworkProxy::NoProxy), int timeout=5)
    {
        QNetworkAccessManager manager;
        QNetworkReply* reply;
        QNetworkRequest request;
        QTimer timer;
        bool istimeout = false;

        timer.setInterval(timeout);
        timer.setSingleShot(true);
        manager.setProxy(proxy);
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        if (method == "get")
        {
            reply = manager.get(request);
        }
        else if (method == "post")
        {
            reply = manager.post(request, postdata.toUtf8());
        }
        else
        {
            return 10001; // Param Error
        }

        QEventLoop eventLoop;
        QObject::connect(&timer, &QTimer::timeout, &eventLoop, [&istimeout]() { istimeout = true; });
        QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            return reply->error(); // 0~500
        }
        if (istimeout)
        {
            return 10000; // Time Out
        }
        response = QString::fromUtf8(reply->readAll());
        eventLoop.deleteLater();
        reply->deleteLater();
        return QNetworkReply::NoError;
    }


};

#endif // UTILS_H
