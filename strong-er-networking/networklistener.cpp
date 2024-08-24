#include "networklistener.h"
#include "networkmanager.h"

NetworkListener::NetworkListener() {
    NetworkManager::the->registerNetworkListener(this);
}

NetworkListener::~NetworkListener() {
    NetworkManager::the->deregisterNetworkListener(this);
}

void NetworkListener::setResponseTimeout() {
    QObject::connect(&responseTimer, &QTimer::timeout, this, &NetworkListener::onTimeout);
    responseTimer.start(responseTimeout);
}

/**
 * @brief Handle new messages from the network
 * @param type
 * @param buffer
 */
void NetworkListener::newMessage(Connection::DataType, QString) {
    responseTimer.stop();
}
