#include "networkmanagerclient.h"

#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QUuid>

#include "connection.h"
#include "networklistener.h"
#include "peermanagerclient.h"

NetworkManagerClient::NetworkManagerClient(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection) :
    NetworkManager(engine, appName, configSection) {
    peerManager = new PeerManagerClient(this);
}

NetworkManagerClient::~NetworkManagerClient() {
}

void NetworkManagerClient::start() {
    if (networkingActive) {
        peerManager->start();
    }
}

void NetworkManagerClient::stop() {
    peerManager->stop();
    NetworkManager::stop();
}
