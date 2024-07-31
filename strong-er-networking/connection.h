#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QTimer>
#include <QElapsedTimer>

class Connection : public QTcpSocket {
    Q_OBJECT
public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    enum DataType {
        // These datatypes are for connections --------
        PlainText,
        Ping,
        Pong,
        Greeting,
        Undefined,
        // Datatypes for our messages below --------
        EditPharmacist,
        AddPharmacist,
        ActiveChangedPharmacist,

        EditPharmacy,
        AddPharmacy,

        EditDoctor,
        AddDoctor,

        EditPatient,
        AddPatient,

        // Integration data types
        PollPrescriptions,
        PollInvoices,

        CorumClearIsEnabled,
        CorumClearEnabled,

        CorumLotsIsEnabled,
        CorumLotsEnabled,

        FredIsEnabled,
        FredEnabled,

        FredNxtIsEnabled,
        FredNxtEnabled,

        FredOfficeIsEnabled,
        FredOfficeEnabled,

        MinfosIsEnabled,
        MinfosEnabled,

        PosWorksIsEnabled,
        PosWorksEnabled,

        RxOneIsEnabled,
        RxOneEnabled,

        ZIsEnabled,
        ZEnabled,

        TotalIncoming,

        IntegrationStatusRequest,
        IntegrationStatusResponse,

        PrescriptionUpdate,
        InvoiceUpdate,

        PosWorksStandaloneIsEnabled,
        PosWorksStandaloneEnabled,
    };

    Connection();
    explicit Connection(qintptr socketDescriptor);
    ~Connection();

    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();
    bool sendMessage(DataType type, const QString &message);
    void stop();

    const QString &getID() const { return id; }
    const QString &getName() const { return name; }
    const QString &getPeerGuid() const { return peerGuid; }

protected:
    void timerEvent(QTimerEvent *timerEvent) override;

private:
    void processGreeting();
    void processData();

    QCborStreamReader reader;
    QCborStreamWriter writer;

    QString greetingMessage;
    ConnectionState state;

    DataType currentDataType;
    int transferTimerId;

    bool isGreetingMessageSent;

    QTimer pingTimer;
    QElapsedTimer pongTime;

    QString buffer;

    QString id;
    QString name;
    QString peerGuid;

signals:
    void readyForUse();
    void newMessage(DataType type, QString buffer);
};

#endif // CONNECTION_H
