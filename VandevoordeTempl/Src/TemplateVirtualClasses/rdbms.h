#ifndef RDBMS_H
#define RDBMS_H

#include "database.h"
#include "executionpolicy.h"

namespace DB {

template<typename EP = ExecutionPolicy>
class RDBMS : public Database
{
public:
    RDBMS() {
    }

    // Database interface
public:
    virtual void greet() override;
};

}

#endif // RDBMS_H
