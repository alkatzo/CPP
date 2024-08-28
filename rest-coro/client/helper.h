#pragma once

#include <QDebug>
#include <QDateTime>

struct Log{
    QString msg;
    bool scope;
    Log() = default;
    Log(const QString &str, bool scope = false) : msg(str), scope(scope) {
        if (scope)
            qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "->";
    }
    ~Log() {
        if (scope) {
            qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "<-";
        }
    }

    inline QDebug &operator<<(auto &&m) {
        return qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << m;
    }
};

#define LOGSCOPE Log logscope(__FUNCTION__, true);
#define LOG Log l(__FUNCTION__);
