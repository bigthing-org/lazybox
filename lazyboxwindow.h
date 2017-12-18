#ifndef LAZYBOXWINDOW_H
#define LAZYBOXWINDOW_H

#include <QMainWindow>




namespace Ui {
class LazyBoxWindow;
}

class LazyBoxWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LazyBoxWindow(QWidget *parent = 0);
    ~LazyBoxWindow();

private slots:
    void on_textEdit_textChanged();

    void on_pushButton_clicked();

private:
    Ui::LazyBoxWindow *ui;
};

#endif // LAZYBOXWINDOW_H
