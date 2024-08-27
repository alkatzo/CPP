#pragma once

#include <QDebug>

#define LOG Log log(__FUNCTION__);

struct Log{
    QString msg;
    Log(const QString &str) : msg(str) {
        qDebug().noquote() << msg << "->";
    }
    ~Log() {
        qDebug().noquote() << msg << "<-";
    }
};
