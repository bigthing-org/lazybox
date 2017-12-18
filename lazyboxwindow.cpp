#include "lazyboxwindow.h"
#include "ui_lazyboxwindow.h"

#include "commonheader.h"
#include "scheduler.hpp"


LazyBoxWindow::LazyBoxWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LazyBoxWindow)
{
    setMinimumSize(500,600);
    setMaximumSize(500,600);
    ui->setupUi(this);
}

LazyBoxWindow::~LazyBoxWindow()
{
    delete ui;
}

void LazyBoxWindow::on_pushButton_clicked()
{
    // QSystemCleaner cleaner;
    // cleaner.Init();
    // cleaner.Analysis();

    // QThunderCleaner cleaner;
    // cleaner.Init();
    // cleaner.Analysis();

    // QBaiduNetDiskCleaner cleaner;
    // cleaner.Init();
    // cleaner.Analysis();

    // QBaiduImeCleaner cleaner;
    // cleaner.Init();
    // cleaner.Analysis();
}

void LazyBoxWindow::on_textEdit_textChanged()
{

}
