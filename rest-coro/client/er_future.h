#pragma once

#include <QCoroTask>
#include "helper.h"

template<typename T>
struct ER_Future
{
    bool await_ready() const noexcept {
        LOG
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        LOG
        promise->handle = h;
    }

    T await_resume() const {
        LOG
        return promise->result;
    }

    struct Promise {
        void setResult(const T& r) {
            LOG
            result = r;
            if (handle) {
                handle.resume();
            }
        }
        T result;
        std::coroutine_handle<> handle = nullptr;
    };

    ER_Future() : promise(new Promise) {
        LOG
    }

public:
    Promise *promise;
};

