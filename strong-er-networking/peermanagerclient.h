#ifndef PEERMANAGERCLIENT_H
#define PEERMANAGERCLIENT_H

#include "peermanager.h"
#include "networkmanagerclient.h"

class PeerManagerClient : public PeerManager
{
public:
    PeerManagerClient(NetworkManager *networkManager);

    virtual void start() override;
    virtual void stop() override;

private:
    void readBroadcastDatagram();

private:
    QSet<QString> connectedApps;
};

#endif // PEERMANAGERCLIENT_H
