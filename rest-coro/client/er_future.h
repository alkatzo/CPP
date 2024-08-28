#pragma once

#include <QCoroTask>
#include <coroutine>
#include "helper.h"

template<typename T>
struct ER_Future
{
    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        promise->handle = h;
    }

    T await_resume() const {
        return promise->result;
    }

    struct Promise {
        Promise() {
            LOG l << this;
        }

        ~Promise() {
            LOG l << this;
        }

        void setResult(const T& r) {
            if (handle && !handle.done()) {
                LOGSCOPE
                result = r;
                handle.resume();
            }
        }
        T result;
        std::coroutine_handle<> handle = nullptr;
    };

    ER_Future() : promise(new Promise) {
        // LOG l << this;
    }

    ER_Future(const ER_Future& f) : promise(f.promise) {
        // LOG l << this;
    }

    ~ER_Future() {
        // LOG l << this;
    }

public:
    std::shared_ptr<Promise> promise;
};

