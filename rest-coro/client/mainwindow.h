#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcoro/qcorotask.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void exec_connect();
    QCoro::Task<int> exec_await();

private slots:
    void on_pbStart_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
