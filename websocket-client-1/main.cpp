#include "mainwindow.h"

#include <QApplication>
#include <QUrl>

#include "websocketclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QUrl url(QStringLiteral("ws://localhost:1234")); // Connect to the server on port 1234
    WebSocketClient client(url);

    MainWindow w;
    w.show();
    return a.exec();
}
