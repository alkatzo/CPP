#include "er_integrationmanager.h"
#include <QSettings>


namespace er {

void IntegrationManager::initialise() {
    qDebug() << __FUNCTION__;

    _server = "http://172.29.208.1:5000/v1";
    _serverDesc = "Ma nigga";
    _timeOut = 5000;
    _pageSize = 1;
}

}
