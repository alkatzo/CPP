#include "websocketserver.h"
#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Qt WebSocket Server"), QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        qDebug() << "WebSocket server listening on port" << port;
    } else {
        qDebug() << "Error starting WebSocket server:" << m_pWebSocketServer->errorString();
    }
}

WebSocketServer::~WebSocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this, pSocket]() {
        pSocket->sendBinaryMessage(pingMessage);
    });
    connect(pSocket, &QWebSocket::pong, [this, pSocket, timer](quint64, const QByteArray &) {
        timer->start(timeoutInterval); // Reset the timer on pong
    });
    m_pingTimers[pSocket] = timer;
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
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        if (message == pingMessage) {
            pClient->sendBinaryMessage(pongMessage);
        }
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        QTimer *timer = m_pingTimers.take(pClient);
        if (timer) {
            timer->stop();
            delete timer;
        }
        pClient->deleteLater();
        qDebug() << "Client disconnected";
    }
}
