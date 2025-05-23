#pragma once

#include <QtConcurrent>
#include <QDebug>
#include <coroutine>

struct Log {
    QString msg;
    Log(const QString &str) : msg(str) {
        qDebug().noquote() << msg << "->";
    }
    ~Log() {
        qDebug().noquote() << msg << "<-";
    }
};

struct Promise;

struct Task {
    using promise_type = Promise;
    using Handle = std::coroutine_handle<promise_type>;

public:
    // Deleted copy constructor and copy assignment operator
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // Constructor from handle
    Task(Handle h) : handle(h) {
        qDebug() << "Task constructor called. Handle:" << handle.address();
    }

private:
    Handle handle = nullptr;
};

struct Awaiter {
    QFuture<int> f;
    bool await_ready() const noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return false;
    }

    void await_suspend(std::coroutine_handle<Promise> handle) const noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        auto *watcher = new QFutureWatcher<int>();
        QObject::connect(watcher, &QFutureWatcherBase::finished, [watcher, handle]() mutable {
            watcher->deleteLater();
            handle.resume();
        });
        watcher->setFuture(f);
    }

    int await_resume() const noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return f.result();
    }
};

struct Awaitable {
    QFuture<int> f;
    auto operator co_await() const noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return Awaiter{f};
    }
};

struct Promise {
    Task get_return_object() {
        qDebug() << __PRETTY_FUNCTION__;
        return Task(Task::Handle::from_promise(*this));
    }

    std::suspend_never initial_suspend() noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return {};
    }

    std::suspend_never final_suspend() noexcept {
        qDebug() << __PRETTY_FUNCTION__;
        return {}; // Suspend at final_suspend to control when it's resumed.
    }

    void unhandled_exception() {
        qDebug() << __PRETTY_FUNCTION__;
        throw;
    }

    void return_value(int res) {
        qDebug() << __PRETTY_FUNCTION__ << res;
    }

    inline auto await_transform(const QFuture<int> &value) {
        qDebug() << __PRETTY_FUNCTION__;
        return Awaitable{value}; // it is OK to return Awaiter from here as well
    }
};
