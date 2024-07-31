#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QMap>
#include <QHostAddress>
#include <QPointer>
#include <QSettings>

#include "connection.h"
#include "server.h"

class NetworkListener;
class PeerManager;
class QQmlApplicationEngine;

class NetworkManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int connectionsCount READ getConnectionsCount NOTIFY connectionsCountChanged);
    Q_PROPERTY(int maxConnectionsSeen READ getMaxConnectionsSeen NOTIFY maxConnectionsSeenChanged);
    Q_PROPERTY(QStringList connectionNames READ getConnectionNames NOTIFY connectionsCountChanged);
    Q_PROPERTY(bool networkingActive READ getNetworkingActive NOTIFY networkingActiveChanged);

public:
    explicit NetworkManager(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection);
    virtual ~NetworkManager();
    static QPointer<NetworkManager> the;

    virtual void start() = 0;
    virtual void stop();

    Q_INVOKABLE void deactivateNetworking();
    Q_INVOKABLE void activateNetworking();

    void newConnection(Connection *connection);
    void sendMessage(Connection::DataType type, QString message);
    void sendMessage(Connection::DataType type, int message);

    int getConnectionsCount() { return peers.values().length(); }
    QStringList getConnectionNames();
    int getMaxConnectionsSeen() { return maxConnectionsSeen; }
    bool getNetworkingActive() { return networkingActive; }
    const QString &getAppName() { return appName; }


    void registerNetworkListener(NetworkListener *listener);
    void deregisterNetworkListener(NetworkListener *listener);

    const QString &getUUID() { return guid; }

public slots:
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();
    void newMessage(Connection::DataType type, QString buffer);

protected:
    void removeConnection(Connection *connection);
    void killConnection(Connection *connection);
    QString getIniProperty(const QString& name);
    void setNetworkingActive(bool active);
    void setMaxConnectionsSeen(int n);

    PeerManager *peerManager = nullptr;
    QMap<QString, Connection*> peers;

    QSettings settings;

    int maxConnectionsSeen = 0;
    bool networkingActive = true;

    QString appName;
    QString configSection;
    static QString guid;

    QList<NetworkListener*> networkListeners;

signals:
    void connectionsCountChanged(int connectionsCount);
    void maxConnectionsSeenChanged();
    void networkingActiveChanged();

};

#endif // NETWORKMANAGER_H
