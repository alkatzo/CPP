#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

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
    void checkPongReceived();

private:
    QWebSocket m_webSocket;
    QTimer m_pingTimer;
    QTimer m_pongTimeoutTimer;
    const QByteArray pingMessage = QByteArray::fromHex("01");
    const QByteArray pongMessage = QByteArray::fromHex("02");
    const int timeoutInterval = 5000; // 5 seconds
};

#endif // WEBSOCKETCLIENT_H
