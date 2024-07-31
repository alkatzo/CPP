#include "mainwindow.h"
#include <QApplication>

#include "../common/log/logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(Logger::redirectDebugMessages);

    MainWindow w;
    w.show();
    return a.exec();
}
