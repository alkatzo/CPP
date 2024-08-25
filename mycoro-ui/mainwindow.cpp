#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtConcurrent>
#include <QThread>

#include <chrono>

int sleepFunction() {
    qDebug() << "Thread started, sleeping for 5 seconds...\n";
    QThread::sleep(5);
    qDebug() << "Thread finished sleeping.\n";
    return 42;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Task<int> MainWindow::exec()
{
    Log log(__FUNCTION__);
    QFuture<int> f = QtConcurrent::run(sleepFunction);
    qDebug() << __FUNCTION__ << "co_await f ...";
    int res = co_await f;
    qDebug() << __FUNCTION__ << "co_await f finished" << res;
    co_return res;
}

Task<int> MainWindow::exec_outer()
{
    Log log(__FUNCTION__);
    int res = co_await exec();
    co_return res;
}

void MainWindow::on_pbStart_clicked()
{
    Log log(__FUNCTION__);
    exec_outer();
    // exec();
}

