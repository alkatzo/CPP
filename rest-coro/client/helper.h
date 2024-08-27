#pragma once

#include <QDebug>
#include <QDateTime>

#define LOG Log log(__FUNCTION__);

struct Log{
    QString msg;
    Log(const QString &str) : msg(str) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "->";
    }
    ~Log() {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << msg << "<-";
    }
};
