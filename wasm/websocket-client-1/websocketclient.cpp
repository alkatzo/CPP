#include "websocketclient.h"

#include <QDebug>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent)
{
    connect(&webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);

    webSocket.open(url);

    pingTimer.setInterval(timeoutInterval);
    connect(&pingTimer, &QTimer::timeout, this, &WebSocketClient::sendPing);
}

void WebSocketClient::onConnected()
{
    qDebug() << QDateTime::currentDateTime() << __FUNCTION__<< "WebSocket connected";

    connect(&webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketClient::onBinaryMessageReceived);

    lastPongTime = QDateTime::currentDateTime();
    pingTimer.start();
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    // Handle text message if necessary
}

void WebSocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    qDebug() << QDateTime::currentDateTime() << __FUNCTION__ << "Binary message received:" << message;
    if (message == pongMessage) {
        qDebug() << QDateTime::currentDateTime() << __FUNCTION__<< "Pong received";
        lastPongTime = QDateTime::currentDateTime();
    }
}

void WebSocketClient::onDisconnected()
{
    pingTimer.stop();
    qDebug() << QDateTime::currentDateTime() << __FUNCTION__<< "WebSocket disconnected";
}

void WebSocketClient::sendPing()
{
    if (checkPongReceived()) {
        qDebug() << QDateTime::currentDateTime() << __FUNCTION__ << "Sending Ping";
        webSocket.sendBinaryMessage(pingMessage);
    }
}

bool WebSocketClient::checkPongReceived()
{
    if (lastPongTime.msecsTo(QDateTime::currentDateTime()) > 2 * timeoutInterval * 1000) {
        qDebug() << QDateTime::currentDateTime() << __FUNCTION__<< "Pong not received in time, closing connection";
        webSocket.close();
        return false;
    }
    return true;
}
