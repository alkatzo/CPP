#include "er_apidefault.h"
#include "er_integrationmanager.h"

namespace er {

ER_Future<QList<ER__people_get_200_response_inner> > ApiDefault::peopleGet(QDateTime dt, int authAttempt) {
    ER_DEFINE_SIGNALS(ApiDefault, peopleGet, ER_DefaultApi, peopleGet, dt)
    post([=](){
        api->peopleGet(dt);
    });

    return f;
}

}
