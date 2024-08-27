#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_future.h"
#include "er_integrationmanager.h"


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
    er::IntegrationManager::initialise();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exec_connect()
{
    er::ApiDefault *api = er::IntegrationManager::erApi<er::ApiDefault>().release();
    api->peopleGet(QDateTime::currentDateTime());
    connect(api, &er::ApiDefault::peopleGetCompletedOK, this, [](QList<er::ER__people_get_200_response_inner> result){
        qDebug() << __FUNCTION__ << "Got results";
        for (const er::ER__people_get_200_response_inner& r : result) {
            qDebug() << r.getFirstName() << r.getLastName() << r.getDateOfBirth();
        }
    });
}

QCoro::Task<int> MainWindow::exec_await()
{
    co_await ER_Future<int>();
    co_return 42;
}

void MainWindow::on_pbStart_clicked()
{
    exec_connect();
}

