#include "websocketclient.h"
#include <QDebug>
#include <QDateTime>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent)
{
    connect(&webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);

    webSocket.open(url);
}

void WebSocketClient::onConnected()
{
    qDebug() << __FUNCTION__ << "WebSocket connected";

    connect(&webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketClient::onBinaryMessageReceived);
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    // Handle text message if necessary
}

void WebSocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    qDebug() << __FUNCTION__ << "Binary message received:" << message.toHex();

    QDataStream stream(message);
    int number;
    stream >> number;

    if (stream.status() == QDataStream::Ok) {
        qDebug() << __FUNCTION__ << "Received integer:" << number;
    } else {
        qDebug() << __FUNCTION__ << "Failed to convert message to integer";
    }
}

void WebSocketClient::onDisconnected()
{
    qDebug() << __FUNCTION__ << "WebSocket disconnected";
}

void WebSocketClient::sendMessage(int number)
{
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream << number;
    webSocket.sendBinaryMessage(message);
    qDebug() << __FUNCTION__ << "Sent binary message:" << message.toHex();
}
