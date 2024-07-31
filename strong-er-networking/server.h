#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QTcpServer {
    Q_OBJECT
public:
    Server();

    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
