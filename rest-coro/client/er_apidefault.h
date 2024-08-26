#pragma once

#include <QObject>
#include "er_base.h"
#include "openapi/ER_DefaultApi.h"

namespace er {

class ApiDefault : public QObject, public RAPI
{
    Q_OBJECT
public:
    explicit ApiDefault() = default;
    ~ApiDefault() = default;

public:
    void peopleGet(QDateTime dt, int authAttempt = 0);
    ER_DECLARE_SHIM_SIGNALS(peopleGet, QList<ER__people_get_200_response_inner>)

    void requestCompleted();
};

}
