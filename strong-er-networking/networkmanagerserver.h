#ifndef NETWORKMANAGERSERVER_H
#define NETWORKMANAGERSERVER_H

#include <QObject>
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QPointer>
#include <QSettings>

#include "networkmanager.h"
#include "connection.h"
#include "server.h"

class NetworkListener;
class PeerManager;
class QQmlApplicationEngine;
class QSettings;

class NetworkManagerServer : public NetworkManager {
    Q_OBJECT
private:
    explicit NetworkManagerServer(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection);
    ~NetworkManagerServer();

public:
    static void newInstance(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection = "") {
        if (!the) {
            the = new NetworkManagerServer(engine, appName, configSection);
        }
    }

    virtual void start();
    virtual void stop();

    int getServerPort() { return server.serverPort(); }

protected:
    Server server;
};

#endif // NETWORKMANAGERSERVER_H
