#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QDateTime>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onBinaryMessageReceived(const QByteArray &message);
    void onDisconnected();
    void sendPing();
    bool checkPongReceived();

private:
    QWebSocket webSocket;
    QTimer pingTimer;
    const QByteArray pingMessage = QByteArray::fromHex("01");
    const QByteArray pongMessage = QByteArray::fromHex("02");
    const int timeoutInterval = 5000; // 5 seconds
    QDateTime lastPongTime;
};

#endif // WEBSOCKETCLIENT_H
