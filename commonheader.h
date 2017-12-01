#ifndef COMMONHEADER_H
#define COMMONHEADER_H

//  公共头文件

#define DEBUG true

#define ARRLEN(X) (int)(sizeof(X) / sizeof(X[0]))

// QT Headers
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>
#include <QtCore/QHash>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegExp>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QtCore>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

enum PLUGIN_TYPE
{
    PLUGIN_TYPE_CLEANER,
};

enum ClEAN_TYPE
{
    CLEAN_TYPE_SYS,
};

// User Defs


#endif // COMMONHEADER_H
