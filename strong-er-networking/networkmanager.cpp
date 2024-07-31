#include "networkmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUuid>

#include "connection.h"
#include "networklistener.h"
#include "peermanager.h"

QPointer<NetworkManager> NetworkManager::the = nullptr;
QString NetworkManager::guid;

NetworkManager::NetworkManager(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection) :
    settings(QSettings::IniFormat,
             QSettings::UserScope,
             "Strong Room Technology",
             "Methadone Dispensing System"),
    appName(appName),
    configSection(configSection) {

    setMaxConnectionsSeen(settings.value(getIniProperty("maxConnectionsSeen"), 0).toInt());
    setNetworkingActive(settings.value(getIniProperty("networkingActive"), true).toBool());

    if (guid.isEmpty()) {
        guid = QUuid::createUuid().toString();
    }

    engine->rootContext()->setContextProperty("NetworkManager", this);
}

NetworkManager::~NetworkManager() {
    stop();
    delete peerManager;
}

void NetworkManager::stop() {
    for (Connection *c : peers.values()) {
        killConnection(c);
    }
    peers.clear();
    emit connectionsCountChanged(getConnectionsCount());
}

void NetworkManager::newConnection(Connection *connection) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(connection, &Connection::errorOccurred, this, &NetworkManager::connectionError);
#endif
    connect(connection, &Connection::disconnected, this, &NetworkManager::disconnected);
    connect(connection, &Connection::readyForUse, this, &NetworkManager::readyForUse);
}

void NetworkManager::readyForUse() {
    Connection *connection = qobject_cast<Connection*>(sender());
    if (!connection) {
        return;
    }
    if (peers.contains(connection->getPeerGuid())) {
        qWarning() << "Connection to app with GUID" << connection->getPeerGuid() << "already exists";
        killConnection(connection);
        return;
    }

    connect(connection,  &Connection::newMessage, this, &NetworkManager::newMessage);

    peers.insert(connection->getPeerGuid(), connection);

    emit connectionsCountChanged(getConnectionsCount());

    if (getConnectionsCount() > maxConnectionsSeen) {
        maxConnectionsSeen = getConnectionsCount();
        settings.setValue(getIniProperty("maxConnectionsSeen"), maxConnectionsSeen);
        emit maxConnectionsSeenChanged();
    }
}

void NetworkManager::disconnected() {
    if (Connection *connection = qobject_cast<Connection*>(sender())) {
        removeConnection(connection);
    }
}

void NetworkManager::connectionError(QAbstractSocket::SocketError /* socketError */) {
    if (Connection *connection = qobject_cast<Connection*>(sender())) {
        removeConnection(connection);
    }
}

// Delay deleting of connections for 10 sec to allow QAbstractSocketPrivate to close correctly
// Prevents crashes when a peer dosconnects and the socket is closed
void NetworkManager::killConnection(Connection *connection) {
    connection->stop();
    QTimer::singleShot(10000, connection, SLOT(deleteLater()));
}

void NetworkManager::removeConnection(Connection *connection) {
    if (peers.contains(connection->getPeerGuid())) {
        peers.remove(connection->getPeerGuid());
        killConnection(connection);
        emit connectionsCountChanged(getConnectionsCount());
    }
}

QString NetworkManager::getIniProperty(const QString &name) {
    return configSection.isEmpty() ?
                name :
                QString("%1/%2").arg(configSection).arg(name);
}

QStringList NetworkManager::getConnectionNames() {
    QStringList list;
    QList<Connection*> conns = peers.values();

    for (int i = 0; i < conns.length(); i++) {
        list.append(conns[i]->getID());
    }
    return list;
}

void NetworkManager::setNetworkingActive(bool active) {
    networkingActive = active;
    settings.setValue(getIniProperty("networkingActive"), active);
    emit networkingActiveChanged();
}

void NetworkManager::setMaxConnectionsSeen(int n) {
    maxConnectionsSeen = n;
    settings.setValue(getIniProperty("maxConnectionsSeen"), n);
    emit maxConnectionsSeenChanged();
}

void NetworkManager::deactivateNetworking() {
    setNetworkingActive(false);
    stop();
}

void NetworkManager::activateNetworking() {
    setNetworkingActive(true);
    start();
}

void NetworkManager::sendMessage(Connection::DataType type, QString message) {
    if (message.isEmpty()) {
        return;
    }

    for (Connection *connection : qAsConst(peers)) {
        connection->sendMessage(type, message);
    }
}

void NetworkManager::sendMessage(Connection::DataType type, int message) {
    sendMessage(type, QString::number(message));
}

void NetworkManager::registerNetworkListener(NetworkListener *listener) {
    if (!networkListeners.contains(listener)) {
        networkListeners.append(listener);
    }
}

void NetworkManager::deregisterNetworkListener(NetworkListener *listener) {
    if (networkListeners.contains(listener)) {
        networkListeners.removeAll(listener);
    }
}

void NetworkManager::newMessage(Connection::DataType type, QString buffer) {
    for (auto listener : networkListeners) {
        listener->newMessage(type, buffer);
    }
}
