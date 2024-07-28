#include "websocketclient.h"

#include <QDebug>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);

    m_webSocket.open(url);

    m_pingTimer.setInterval(timeoutInterval);
    connect(&m_pingTimer, &QTimer::timeout, this, &WebSocketClient::sendPing);

    connect(&m_pongTimeoutTimer, &QTimer::timeout, this, &WebSocketClient::checkPongReceived);
}

void WebSocketClient::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketClient::onBinaryMessageReceived);

    m_pingTimer.start();
    m_pongTimeoutTimer.start(timeoutInterval);

    qDebug() << "WebSocket connected";
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    // Handle text message if necessary
}

void WebSocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    if (message == pongMessage) {
        qDebug() << "Pong received";
        m_pongTimeoutTimer.start(timeoutInterval); // Reset the timeout timer on pong
    }
}

void WebSocketClient::onDisconnected()
{
    m_pingTimer.stop();
    m_pongTimeoutTimer.stop();
    qDebug() << "WebSocket disconnected";
}

void WebSocketClient::sendPing()
{
    m_webSocket.sendBinaryMessage(pingMessage);
}

void WebSocketClient::checkPongReceived()
{
    qDebug() << "Pong not received in time, closing connection";
    m_webSocket.close();
}
