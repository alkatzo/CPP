#include "networkmanagerserver.h"

#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QUuid>

#include "connection.h"
#include "networklistener.h"
#include "peermanagerserver.h"

NetworkManagerServer::NetworkManagerServer(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection) :
    NetworkManager(engine, appName, configSection) {
    peerManager = new PeerManagerServer(this);
}

NetworkManagerServer::~NetworkManagerServer() {
}

void NetworkManagerServer::start() {
    if (networkingActive) {
        server.resumeAccepting();
        qDebug() << __FUNCTION__ << "Server is listening on port" << server.serverPort();
        peerManager->start();
    }
}

void NetworkManagerServer::stop() {
    peerManager->stop();
    server.pauseAccepting();
    NetworkManager::stop();
}
