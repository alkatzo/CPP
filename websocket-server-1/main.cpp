#include <QCoreApplication>

#include "websocketserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebSocketServer server(1234); // Start the server on port 1234

    return a.exec();
}
