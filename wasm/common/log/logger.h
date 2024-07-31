#ifndef LOGGER_H
#define LOGGER_H

#include <QMutex>
#include <QStandardPaths>

class Logger {

public:
    explicit Logger();
    ~Logger();
    static void redirectDebugMessages(QtMsgType type,
                                      const QMessageLogContext &context,
                                      const QString &msg);

};

#endif // LOGGER_H
