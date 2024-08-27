#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <chrono>

#include "er_future.h"
#include "er_integrationmanager.h"
#include "helper.h"

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

QCoro::Task<void> MainWindow::exec_await()
{
    LOG
    QList<er::ER__people_get_200_response_inner> result = co_await exec_awaitCo();
    qDebug() << __FUNCTION__ << "Got results";
    for (const er::ER__people_get_200_response_inner& r : result) {
        qDebug() << r.getFirstName() << r.getLastName() << r.getDateOfBirth();
    }
    co_return;
}

QCoro::Task<QList<er::ER__people_get_200_response_inner>> MainWindow::exec_awaitCo()
{
    LOG
    auto api = er::IntegrationManager::erApi<er::ApiDefault>().release();

    QList<er::ER__people_get_200_response_inner> result;
    for (int i = 1; i < 4; i++) {
        qDebug() << "Calling api->peopleGet() with page" << i;
        result.append(co_await api->peopleGet(QDateTime::currentDateTime(), i));
    }

    co_return result;
}

void MainWindow::on_pbStart_clicked()
{
    LOG
    exec_await();
    // exec_connect();
}

