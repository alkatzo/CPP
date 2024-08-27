#pragma once

#include <QCoroTask>

template<typename T>
struct ER_Future
{
    template<typename T_>
    class WaitForFinishedOperation {
    public:
        explicit WaitForFinishedOperation() {}
        Q_DISABLE_COPY(WaitForFinishedOperation)
        WaitForFinishedOperation(WaitForFinishedOperation &&) noexcept = default;
        WaitForFinishedOperation &operator=(WaitForFinishedOperation &&) noexcept = default;

        bool await_ready() const noexcept {
            return false;
        }

        void await_suspend(std::coroutine_handle<> awaitingCoroutine) {
        }

        T await_resume() const {
            return {};
        }
    };

public:
    ER_Future() {}

public:
    T result;
};


namespace QCoro { namespace detail {

template<typename T>
struct awaiter_type<ER_Future<T>> {
    using type = typename ER_Future<T>::WaitForFinishedOperation;
};

}}
