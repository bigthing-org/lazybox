/********************************************************************************
** Form generated from reading UI file 'lazyboxwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAZYBOXWINDOW_H
#define UI_LAZYBOXWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LazyBoxWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LazyBoxWindow)
    {
        if (LazyBoxWindow->objectName().isEmpty())
            LazyBoxWindow->setObjectName(QStringLiteral("LazyBoxWindow"));
        centralWidget = new QWidget(LazyBoxWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 170, 51, 20));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(0, 10, 451, 151));
        LazyBoxWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(LazyBoxWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 448, 17));
        LazyBoxWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LazyBoxWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        LazyBoxWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LazyBoxWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        LazyBoxWindow->setStatusBar(statusBar);

        retranslateUi(LazyBoxWindow);

        QMetaObject::connectSlotsByName(LazyBoxWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LazyBoxWindow)
    {
        LazyBoxWindow->setWindowTitle(QApplication::translate("LazyBoxWindow", "LazyBoxWindow", Q_NULLPTR));
        pushButton->setText(QApplication::translate("LazyBoxWindow", "test1", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LazyBoxWindow: public Ui_LazyBoxWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAZYBOXWINDOW_H
