#pragma once

#include <QCoroTask>
#include "qcoro/macros_p.h"

template<typename T>
struct ER_Future
{
    Q_DISABLE_COPY(ER_Future)
    QCORO_DEFAULT_MOVE(ER_Future)

    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> awaitingCoroutine) {
        awaitingCoroutine.resume();
    }

    T await_resume() const {
        return {};
    }

public:
    ER_Future() {}

public:
    T result;
};

