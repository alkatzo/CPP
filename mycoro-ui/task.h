#pragma once

#include <QtConcurrent>
#include <QDebug>
#include <coroutine>

struct Log{
    QString msg;
    Log(const QString &str) : msg(str) {
        qDebug().noquote() << msg << "->";
    }
    ~Log() {
        qDebug().noquote() << msg << "<-";
    }
};

template <typename T>
struct Promise;

template <typename T>
struct Task {
    using promise_type = Promise<T>;

public:
    // Deleted copy constructor and copy assignment operator
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task &&task) : handle(task.handle) {
        task.handle = nullptr;
    }

    // Constructor from handle
    Task(std::coroutine_handle<promise_type> h) : handle(h) {
        qDebug() << __FUNCTION__;
        qDebug() << "Task constructor called. Handle:" << handle.address();
    }

    auto operator co_await() const noexcept;

private:
    std::coroutine_handle<promise_type> handle = nullptr;
};

template <typename T>
struct AwaiterQFuture {
    QFuture<T> f;

    bool await_ready() const noexcept {
        qDebug() << __FUNCTION__;
        return f.isFinished() || f.isCanceled();
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept {
        qDebug() << __FUNCTION__;
        auto *watcher = new QFutureWatcher<T>();
        QObject::connect(watcher, &QFutureWatcherBase::finished, [watcher, handle]() mutable {
            watcher->deleteLater();
            handle.resume();
        });
        watcher->setFuture(f);
    }

    T await_resume() const noexcept {
        qDebug() << __FUNCTION__;
        return f.result();
    }
};

template <typename T>
struct AwaitableQFuture {
    QFuture<T> f;
    auto operator co_await() const noexcept {
        qDebug() << __FUNCTION__;
        return AwaiterQFuture<T>{f};
    }
};

template<typename T>
struct awaitable_type;

template<typename T>
using awaitable_type_t = typename awaitable_type<T>::type;

template<typename T>
struct awaitable_type<QFuture<T>> {
    using type = AwaitableQFuture<T>;
};


template <typename T>
struct Promise {
    Task<T> get_return_object() {
        qDebug() << __FUNCTION__;
        return Task<T>(std::coroutine_handle<Promise>::from_promise(*this));
    }

    std::suspend_never initial_suspend() noexcept {
        qDebug() << __FUNCTION__;
        return {};
    }

    std::suspend_never final_suspend() noexcept {
        qDebug() << __FUNCTION__;
        return {};
    }

    void unhandled_exception() {
        qDebug() << __FUNCTION__;
        throw;
    }

    void return_value(T res) {
        qDebug() << __FUNCTION__ << res;
    }

    // co_await exp calls this
    template<typename E, typename AwaitableT = awaitable_type_t<std::remove_cvref_t<E>>>
    inline auto await_transform(E &&value) {
        qDebug() << __FUNCTION__;
        return AwaitableT{value}; // it is OK to return Awaiter from here as well
    }

    // override for when expr is a Task rvalue
    template<typename T>
    auto await_transform(Task<T> &&task) {
        return std::forward<Task<T>>(task);
    }

    // override for when expr is a Task ref
    template<typename T>
    auto await_transform(Task<T> &task) {
        return task;
    }
};

template<typename T>
inline auto Task<T>::operator co_await() const noexcept {
    //! Specialization of the TaskAwaiterBase that returns the promise result by value
    struct AwaiterTask {
        bool await_ready() const noexcept {
            qDebug() << __FUNCTION__;
            return false;
        }

        void await_suspend(std::coroutine_handle<> handle) const noexcept {
            qDebug() << __FUNCTION__;
        }

        T await_resume() const noexcept {
            qDebug() << __FUNCTION__;
            return {};
        }
    };

    return AwaiterTask{};
}
