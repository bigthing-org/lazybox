#include "commonheader.h"
#include "scheduler.h"
#include "utils.h"

#include <QtWidgets/QApplication>
#include "lazyboxwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LazyBoxWindow win;
    win.show();

    for (int i = 0 ;i<25;i++)
    {
        qDebug() << i << ":" << QStandardPaths::writableLocation((QStandardPaths::StandardLocation)i);
    }

    QProcessEnvironment envs = QProcessEnvironment::systemEnvironment();
    for (const QString& key : envs.keys())
    {
        qDebug() << key.toLower() << ":" << envs.value(key);
    }

    qDebug() << Utils::GetPaths();

    return app.exec();
}

