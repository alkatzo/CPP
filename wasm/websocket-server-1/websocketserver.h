#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
#include <QHash>

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer();

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();

private:
    QWebSocketServer webSocketServer;
    QList<QWebSocket *> clients;
    QHash<QWebSocket *, QTimer *> pingTimers;
    const QByteArray pingMessage = QByteArray::fromHex("01");
    const QByteArray pongMessage = QByteArray::fromHex("02");
    const int timeoutInterval = 5000; // 5 seconds
};

#endif // WEBSOCKETSERVER_H
