#include <QCoreApplication>
#include <QtLogging>

#include "websocketserver.h"
#include "../common/log/logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInstallMessageHandler(Logger::redirectDebugMessages);

    WebSocketServer server(1234); // Start the server on port 1234

    return a.exec();
}
