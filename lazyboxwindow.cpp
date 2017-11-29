#include "lazyboxwindow.h"
#include "ui_lazyboxwindow.h"

#include "commonheader.h"
#include "scheduler.h"
#include "config.h"

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
    auto l = []() { qDebug() << "lambda ok"; };
    QTask<decltype(l)> t(l);
    t.run();
    ui->textEdit->setText("ok\nokok\nokok\nokok\nokok\nokok\nokok\nokok\nokok\nokok\nok");
}

void LazyBoxWindow::on_textEdit_textChanged()
{

}
