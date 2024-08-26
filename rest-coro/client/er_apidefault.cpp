#include "er_apidefault.h"
#include "er_integrationmanager.h"

namespace er {

void ApiDefault::peopleGet(QDateTime dt, int authAttempt) {
    ER_DEFINE_SIGNALS(ApiDefault, peopleGet, ER_DefaultApi, peopleGet, dt)
    post([=](){
        api->peopleGet(dt);
    });
}

}
