#include "connection.h"

#include <QHostInfo>
#include <QtNetwork>

#include "networkmanager.h"

static const int TransferTimeout = 30 * 1000;
static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;

Connection::Connection() : QTcpSocket(), writer(this) {
    greetingMessage = tr(QString(NetworkManager::the->getUUID() + " " + NetworkManager::the->getAppName() + ": " + QHostInfo::localHostName()).toLocal8Bit().data());
    state = WaitingForGreeting;
    currentDataType = Undefined;
    transferTimerId = -1;
    isGreetingMessageSent = false;
    pingTimer.setInterval(PingInterval);

    connect(this, &QTcpSocket::readyRead, this, &Connection::processReadyRead);
    connect(this, &QTcpSocket::disconnected, &pingTimer, &QTimer::stop);
    connect(&pingTimer, &QTimer::timeout, this, &Connection::sendPing);
    connect(this, &QTcpSocket::connected, this, &Connection::sendGreetingMessage);
}

Connection::Connection(qintptr socketDescriptor) : Connection() {
    setSocketDescriptor(socketDescriptor);
    reader.setDevice(this);
}

Connection::~Connection() {
}

bool Connection::sendMessage(DataType type, const QString &message) {
    if (message.isEmpty()) {
        return false;
    }
    writer.startMap(1);
    writer.append(type);
    writer.append(message);
    writer.endMap();
    return true;
}

void Connection::stop() {
    pingTimer.stop();
}

void Connection::timerEvent(QTimerEvent *timerEvent) {
    if (timerEvent->timerId() == transferTimerId) {
        abort();
        killTimer(transferTimerId);
        transferTimerId = -1;
    }
}

void Connection::processReadyRead() {
    // we've got more data, let's parse
    reader.reparse();
    while (reader.lastError() == QCborError::NoError) {
        if (state == WaitingForGreeting) {
            // indicates protocol error
            if (!reader.isArray()) {
                break;
            }

            reader.enterContainer();
            state = ReadingGreeting;
        } else if (reader.containerDepth() == 1) {
            // Current state: no command read
            // Next state: read command ID
            if (!reader.hasNext()) {
                reader.leaveContainer();
                disconnectFromHost();
                return;
            }
            // protocol error
            if (!reader.isMap() || !reader.isLengthKnown() || reader.length() != 1) {
                break;
            }
            reader.enterContainer();
        } else if (currentDataType == Undefined) {
            // read command ID
            // Next: read command payload

            // protocol error
            if (!reader.isInteger()) {
                break;
            }
            currentDataType = DataType(reader.toInteger());
            reader.next();
        } else {
            // read command payload
            if (reader.isString()) {
                auto r = reader.readString();
                buffer += r.data;
                if (r.status != QCborStreamReader::EndOfString) {
                    continue;
                }
            } else if (reader.isNull()) {
                reader.next();
            } else {
                break;
            }

            // Next: no command read
            reader.leaveContainer();
            if (transferTimerId != -1) {
                killTimer(transferTimerId);
                transferTimerId = -1;
            }

            if (state == ReadingGreeting) {
                // protocol error
                if (currentDataType != Greeting) {
                    break;
                }
                processGreeting();
            } else {
                processData();
            }
        }
    }

    // parse error
    if (reader.lastError() != QCborError::EndOfFile) {
        abort();
    }

    if (transferTimerId != -1 && reader.containerDepth() > 1) {
        transferTimerId = startTimer(TransferTimeout);
    }
}

void Connection::sendPing() {
    if (pongTime.elapsed() > PongTimeout) {
        abort();
        return;
    }

    writer.startMap(1);
    writer.append(Ping);
    writer.append(nullptr);
    writer.endMap();
}

void Connection::sendGreetingMessage() {
    if (isGreetingMessageSent) {
        return;
    }
    qDebug() << __FUNCTION__ << greetingMessage;

    writer.startArray();

    writer.startMap(1);
    writer.append(Greeting);
    writer.append(greetingMessage);
    writer.endMap();
    isGreetingMessageSent = true;

    if (!reader.device()) {
        reader.setDevice(this);
    }
}

void Connection::processGreeting() {
    name = buffer;

    int indexOfSpace = buffer.indexOf(" ");
    if ((indexOfSpace == -1 && indexOfSpace >= buffer.length() - 1) || !isValid()) {
        abort();
        return;
    }

    peerGuid = buffer.left(indexOfSpace);
    name = buffer.mid(indexOfSpace + 1);
    id = name + '@' + peerAddress().toString().remove("::ffff:") + ':' + QString::number(peerPort());
    currentDataType = Undefined;
    buffer.clear();

    sendGreetingMessage();

    pingTimer.start();
    pongTime.start();
    state = ReadyForUse;
    emit readyForUse();
    qDebug() << __FUNCTION__ << "guid:" << peerGuid << "name:" << name << "id:" << id;
}

void Connection::processData() {
    switch (currentDataType) {
        case Ping:
            writer.startMap(1);
            writer.append(Pong);
            writer.append(nullptr);
            writer.endMap();
            break;
        case Pong:
            pongTime.restart();
            break;
        default:
            emit newMessage(currentDataType, buffer);
            break;
    }

    currentDataType = Undefined;
    buffer.clear();
}
