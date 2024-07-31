#ifndef NETWORKMANAGERCLIENT_H
#define NETWORKMANAGERCLIENT_H

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

class NetworkManagerClient : public NetworkManager {
    Q_OBJECT
public:
    explicit NetworkManagerClient(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection);
    ~NetworkManagerClient();

    static void newInstance(QQmlApplicationEngine *engine, const QString &appName, const QString &configSection = "") {
        if (!the) {
            the = new NetworkManagerClient(engine, appName, configSection);
        }
    }

    void start();
    void stop();
};

#endif // NETWORKMANAGERCLIENT_H
