#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new WebSocketClient(QUrl(QStringLiteral("ws://localhost:1234")), this)) // Initialize WebSocketClient
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client; // Clean up WebSocketClient
}

void MainWindow::on_pushButton_clicked()
{
    int number = 42; // Example number to send
    client->sendMessage(number); // Send message to server
}
