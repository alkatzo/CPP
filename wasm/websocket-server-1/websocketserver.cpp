#include "websocketserver.h"
#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , webSocketServer(QStringLiteral("Qt WebSocket Server"), QWebSocketServer::NonSecureMode, this)
{
    if (webSocketServer.listen(QHostAddress::Any, port)) {
        connect(&webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        qDebug() << "WebSocket server listening on port" << port;
    } else {
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

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this, pSocket]() {
        pSocket->sendBinaryMessage(pingMessage);
    });
    connect(pSocket, &QWebSocket::pong, [this, pSocket, timer](quint64, const QByteArray &) {
        timer->start(timeoutInterval); // Reset the timer on pong
    });
    pingTimers[pSocket] = timer;
    timer->start(timeoutInterval);

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
        if (message == pingMessage) {
            qDebug() << __FUNCTION__ << "Sending Pong";
            pClient->sendBinaryMessage(pongMessage);
        }
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        clients.removeAll(pClient);
        QTimer *timer = pingTimers.take(pClient);
        if (timer) {
            timer->stop();
            delete timer;
        }
        pClient->deleteLater();
        qDebug() << "Client disconnected";
    }
}
