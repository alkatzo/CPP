#include <QCoreApplication>
#include <QtConcurrent>
#include <QThread>
#include <QDebug>

#include <coroutine>
#include <chrono>

#include "task.h"

int sleepFunction() {
    qDebug() << "Thread started, sleeping for 5 seconds...\n";
    QThread::sleep(std::chrono::seconds(5));
    qDebug() << "Thread finished sleeping.\n";
    return 42;
}

Task exec() {
    QFuture<int> f = QtConcurrent::run(sleepFunction);
    qDebug() << __PRETTY_FUNCTION__ << "co_await f ...";
    int res = co_await f;
    qDebug() << __PRETTY_FUNCTION__ << "co_await f finished" << res;
    co_return res;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << __PRETTY_FUNCTION__ << "Calling exec()";
    exec();
    qDebug() << __PRETTY_FUNCTION__ << "Finished exec()";


    return a.exec();
}
