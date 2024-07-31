#include "peermanager.h"

#include <QtNetwork>

#include "networkmanager.h"


PeerManager::PeerManager(NetworkManager *networkManager) {
    this->networkManager = networkManager;
    broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

PeerManager::~PeerManager() {
    broadcastSocket.close();
}
