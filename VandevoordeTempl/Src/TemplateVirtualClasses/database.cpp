#include "database.h"
#include "executionpolicy.h"
#include "rdbms.h"

void Database::make_db() {
    if (!the) {
        the = new DB::RDBMS;
    }
}
