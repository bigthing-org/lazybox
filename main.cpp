#include <QApplication>
#include "commonheader.h"
#include "lazyboxwindow.h"
#include "lazyboxconsole.h"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    /*
     *     for (int i = 0 ;i<25;i++)
            {
                qDebug() << i << ":" << QStandardPaths::writableLocation((QStandardPaths::StandardLocation)i);
            }

            QProcessEnvironment envs = QProcessEnvironment::systemEnvironment();
            for (const QString& key : envs.keys())
            {
                qDebug() << key.toLower() << ":" << envs.value(key);
            }

            qDebug() << Utils::GetPaths();
      */

    int startMode = 0; // 0:gui 1:console
    for (int i = 0; i < argc; i++)
    {
        if (!strcmpi(argv[i], "-console"))
        {
            startMode = 1;
        }
        if (!strcmpi(argv[i], "-gui"))
        {
            startMode = 2;
        }
    }

    if (startMode == 2)
    {
        QApplication app(argc, argv);
        LazyBoxWindow win;
        win.show();
        return app.exec();
    }
    else if (startMode == 1)
    {
        lazyboxconsole console;
        return console.loop();
    }
    return -1;
}

