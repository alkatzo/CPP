#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QHash>

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer();
    void sendMessageToAllClients(int number);

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();

private:
    QWebSocketServer webSocketServer;
    QList<QWebSocket *> clients;
};

#endif // WEBSOCKETSERVER_H
