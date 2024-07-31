#include "logger.h"

#include <QDateTime>
#include <QThread>
#include <QTextStream>

Logger::Logger() { }

Logger::~Logger() { }

void Logger::redirectDebugMessages(QtMsgType type,
                                   const QMessageLogContext &context,
                                   const QString &msg) {
    Q_UNUSED(context)

    QString outputText;

    switch (type) {
        case QtMsgType::QtDebugMsg:
            outputText += "Debug: ";
            break;
        case QtMsgType::QtInfoMsg:
            outputText += "Info: ";
            break;
        case QtMsgType::QtWarningMsg:
            outputText += "Warning: ";
            break;
        case QtMsgType::QtCriticalMsg:
            outputText += "Critical: ";
            break;
        case QtMsgType::QtFatalMsg:
            outputText += "Fatal: ";
            break;
        default:
            break;
    }

        outputText += QString("%1 (%2) %3\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh::mm::ss.zzz")).arg((long long)QThread::currentThreadId()).arg(msg);

        fprintf(stdout, "%s", outputText.toLocal8Bit().constData());
        fflush(stdout);
}
