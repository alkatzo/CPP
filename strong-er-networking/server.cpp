#include "server.h"

#include <QtNetwork>

#include "connection.h"
#include "networkmanager.h"

Server::Server() : QTcpServer() {
    listen(QHostAddress::Any);
}

void Server::incomingConnection(qintptr socketDescriptor) {
    Connection *connection = new Connection(socketDescriptor);
    NetworkManager::the->newConnection(connection);
}
