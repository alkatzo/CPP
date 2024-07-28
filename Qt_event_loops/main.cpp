#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create a timer that fires every second
    QTimer timer;
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, [](){
        qDebug() << "Timer expired";
    });

    // Start the timer and enter the event loop
    timer.start();
    app.exec();

    // Create a new event loop and a timer that fires every half second
    QEventLoop loop;
    QTimer nestedTimer;
    nestedTimer.setInterval(500);
    QObject::connect(&nestedTimer, &QTimer::timeout, [&loop](){
        qDebug() << "Nested timer expired";
        loop.quit();
    });

    // Start the nested timer and enter the nested event loop
    nestedTimer.start();
    loop.exec();

    qDebug() << "Exiting program";
    return 0;
}
