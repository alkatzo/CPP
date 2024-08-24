#include "peermanagerclient.h"

#include <QtNetwork>


PeerManagerClient::PeerManagerClient(NetworkManager *networkManager) :
    PeerManager(networkManager) {
}

void PeerManagerClient::start() {
    connect(&broadcastSocket, &QUdpSocket::readyRead, this, &PeerManagerClient::readBroadcastDatagram);
}

void PeerManagerClient::stop() {
    disconnect(&broadcastSocket);
}

void PeerManagerClient::readBroadcastDatagram() {
    while (broadcastSocket.hasPendingDatagrams()) {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(broadcastSocket.pendingDatagramSize());
        if (broadcastSocket.readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort) == -1) {
            continue;
        }

        QString guid;
        int senderServerPort;
        // decode the datagram
        QCborStreamReader reader(datagram);
        if (reader.lastError() != QCborError::NoError || !reader.isArray()) {
            continue;
        }
        if (!reader.isLengthKnown() || reader.length() != 2) {
            continue;
        }

        reader.enterContainer();
        if (reader.lastError() != QCborError::NoError || !reader.isString()) {
            continue;
        }
        guid = reader.readString().data;
        reader.next();
        if (reader.lastError() != QCborError::NoError || !reader.isUnsignedInteger()) {
            continue;
        }
        senderServerPort = reader.toInteger();

        if (guid != networkManager->getUUID() && !connectedApps.contains(guid)) {
            Connection *connection = new Connection();
            networkManager->newConnection(connection);
            qDebug() << __FUNCTION__ << "Connecting to" << guid << senderIp << ":" << senderServerPort;
            connection->connectToHost(senderIp, senderServerPort);
            if (connection->waitForConnected(5000)) {
                qDebug() << __FUNCTION__ << "Connected to" << guid << senderIp << ":" << senderServerPort;
                connectedApps.insert(guid);
                connect(connection, &Connection::disconnected, [=]() {
                    qDebug() << __FUNCTION__ << "Disconnected from" << guid << senderIp << ":" << senderServerPort;
                    connectedApps.remove(guid);
                });
            }
        }
    }
}
