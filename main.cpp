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

    return app.exec();
}

