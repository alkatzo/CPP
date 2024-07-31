#ifndef PEERMANAGERSERVER_H
#define PEERMANAGERSERVER_H

#include "peermanager.h"

#include <QTimer>

#include "networkmanagerserver.h"

class PeerManagerServer : public PeerManager
{
public:
    PeerManagerServer(NetworkManager *networkManager);

    virtual void start() override;
    virtual void stop() override;

private:
    void updateAddresses();
    void sendBroadcastDatagram();

private:
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;

    QTimer broadcastTimer;

    static const qint32 BroadcastInterval = 2000;

};

#endif // PEERMANAGERSERVER_H
