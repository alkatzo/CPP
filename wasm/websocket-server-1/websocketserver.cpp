#include "websocketserver.h"

#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , webSocketServer(QStringLiteral("Qt WebSocket Server"), QWebSocketServer::NonSecureMode, this)
{
    if (webSocketServer.listen(QHostAddress::Any, port)) {
        connect(&webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        qDebug() << "WebSocket server listening on port" << port;
    }
    else {
        qDebug() << "Error starting WebSocket server:" << webSocketServer.errorString();
    }
}

WebSocketServer::~WebSocketServer()
{
    webSocketServer.close();
    qDeleteAll(clients.begin(), clients.end());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = webSocketServer.nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    clients << pSocket;

    qDebug() << "New client connected";
}

void WebSocketServer::processTextMessage(const QString &message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        // Handle text message if necessary
    }
}

void WebSocketServer::processBinaryMessage(const QByteArray &message)
{
    qDebug() << __FUNCTION__;
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        QDataStream stream(message);
        int number;
        stream >> number;

        if (stream.status() == QDataStream::Ok) {
            qDebug() << QDateTime::currentDateTime() << __FUNCTION__ << "Received integer:" << number;
        }
        else {
            qDebug() << QDateTime::currentDateTime() << __FUNCTION__ << "Failed to convert message to integer";
        }
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
        qDebug() << "Client disconnected";
    }
}

void WebSocketServer::sendMessageToAllClients(int number)
{
    QString message = QString::number(number);
    for (QWebSocket *client : std::as_const(clients)) {
        client->sendTextMessage(message);
    }
    qDebug() << "Sent message to all clients:" << message;
}
