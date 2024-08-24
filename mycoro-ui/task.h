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
    using Handle = std::coroutine_handle<promise_type>;

public:
    // Deleted copy constructor and copy assignment operator
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // Constructor from handle
    Task(Handle h) : handle(h) {
        Log log(__FUNCTION__);
        qDebug() << "Task constructor called. Handle:" << handle.address();
    }

private:
    Handle handle = nullptr;
};

template <typename T>
struct AwaiterTask {
    bool await_ready() const noexcept {
        Log log(__FUNCTION__);
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept {
        Log log(__FUNCTION__);
    }

    T await_resume() const noexcept {
        Log log(__FUNCTION__);
    }
};

template <typename T>
struct AwaiterQFuture {
    QFuture<T> f;
    bool await_ready() const noexcept {
        Log log(__FUNCTION__);
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept {
        Log log(__FUNCTION__);
        auto *watcher = new QFutureWatcher<T>();
        QObject::connect(watcher, &QFutureWatcherBase::finished, [watcher, handle]() mutable {
            watcher->deleteLater();
            handle.resume();
        });
        watcher->setFuture(f);
    }

    T await_resume() const noexcept {
        Log log(__FUNCTION__);
        return f.result();
    }
};

template <typename T>
struct AwaitableQFuture {
    QFuture<T> f;
    auto operator co_await() const noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return AwaiterQFuture<T>{f};
    }
};

template <typename T>
struct Promise {
    Task<T> get_return_object() {
        Log log(__FUNCTION__);
        return Task<T>(Task<T>::Handle::from_promise(*this));
    }

    std::suspend_never initial_suspend() noexcept {
        Log log(__FUNCTION__);
        return {};
    }

    std::suspend_never final_suspend() noexcept {
        Log log(__FUNCTION__);
        return {};
    }

    void unhandled_exception() {
        Log log(__FUNCTION__);
        throw;
    }

    void return_value(T res) {
        qDebug() << __FUNCTION__ << res;
    }

    inline auto await_transform(const QFuture<T> &value) {
        qDebug() << __FUNCTION__;
        return AwaitableQFuture<T>{value}; // it is OK to return Awaiter from here as well
    }
};
