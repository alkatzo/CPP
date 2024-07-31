#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QObject>
#include <QUdpSocket>

class NetworkManager;

class PeerManager : public QObject {
    Q_OBJECT
public:
    explicit PeerManager(NetworkManager *networkManager);
    virtual ~PeerManager();

    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    QUdpSocket broadcastSocket;

    NetworkManager *networkManager;

    static const unsigned broadcastPort = 45000;
};

#endif // PEERMANAGER_H
