#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include "connection.h"

class NetworkListener : public QObject {
public:
    explicit NetworkListener();
    ~NetworkListener();

    virtual void setResponseTimeout();
    virtual void newMessage(Connection::DataType type, QString buffer);
    virtual void onTimeout() {};

private:
    QTimer responseTimer;

    static constexpr int responseTimeout = 3000;
};

#endif // NETWORKLISTENER_H
