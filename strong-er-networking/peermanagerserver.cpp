#include "peermanagerserver.h"

#include <QtNetwork>

PeerManagerServer::PeerManagerServer(NetworkManager *networkManager)
    : PeerManager(networkManager) {

    broadcastTimer.setInterval(BroadcastInterval);
    connect(&broadcastTimer, &QTimer::timeout, this, &PeerManagerServer::sendBroadcastDatagram);
    updateAddresses();
}

void PeerManagerServer::start() {
    broadcastTimer.start();
}

void PeerManagerServer::stop() {
    broadcastTimer.stop();
}

void PeerManagerServer::updateAddresses() {
    broadcastAddresses.clear();
    ipAddresses.clear();
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        const QList<QNetworkAddressEntry> entries = interface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
                broadcastAddresses << broadcastAddress;
                ipAddresses << entry.ip();
            }
        }
    }
}

void PeerManagerServer::sendBroadcastDatagram() {
    NetworkManagerServer *nm = static_cast<NetworkManagerServer*>(networkManager);
    QByteArray datagram; {
        QCborStreamWriter writer(&datagram);
        writer.startArray(2);
        writer.append(nm->getUUID());
        writer.append(nm->getServerPort());
        writer.endArray();
    }

    bool validBroadcastAddresses = true;
    for (const QHostAddress &address : qAsConst(broadcastAddresses)) {
        if (broadcastSocket.writeDatagram(datagram, address, broadcastPort) == -1) {
            validBroadcastAddresses = false;
        }
    }

    if (!validBroadcastAddresses) {
        updateAddresses();
    }
}
