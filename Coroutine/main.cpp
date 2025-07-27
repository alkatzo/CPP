#include <coroutine>
#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <functional>

// Simple Task type for coroutines
template <typename T = void>
struct Task;

struct FinalAwaiter {
    bool await_ready() noexcept { return false; }

    template <typename PromiseType>
    void await_suspend(std::coroutine_handle<PromiseType> h) noexcept {
        if (h.promise().continuation)
            h.promise().continuation.resume();
    }

    void await_resume() noexcept {}
};

struct TaskPromiseBase {
    std::coroutine_handle<> continuation = nullptr;

    auto initial_suspend() { return std::suspend_always{}; }

    auto final_suspend() noexcept { return FinalAwaiter{}; }

    void unhandled_exception() { std::terminate(); }
};

template <>
struct Task<void> {
    struct promise_type : TaskPromiseBase {
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        void return_void() {}
    };

    std::coroutine_handle<promise_type> coro;

    explicit Task(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Task() {
        if (coro) coro.destroy();
    }

    void start() {
        coro.resume();
    }
};

// Awaitable that posts itself to the event loop after a delay
struct AsyncSleep {
    int milliseconds;
    std::coroutine_handle<> handle;

    AsyncSleep(int ms) : milliseconds(ms) {}

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h);
    void await_resume() const noexcept {}

    // Static event queue
    static std::queue<std::function<void()>> eventQueue;
};

std::queue<std::function<void()>> AsyncSleep::eventQueue;

void AsyncSleep::await_suspend(std::coroutine_handle<> h) {
    handle = h;
    // Simulate async work: after delay, post to event queue
    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        eventQueue.push([h = this->handle]() { h.resume(); });
    }).detach();
}

// Coroutine using Awaitable
Task<> worker(int id, int delay_ms) {
    std::cout << "Worker " << id << " started\n";
    co_await AsyncSleep(delay_ms);
    std::cout << "Worker " << id << " resumed after " << delay_ms << "ms\n";
}

// Simulated event loop
void runEventLoop() {
    while (!AsyncSleep::eventQueue.empty()) {
        auto task = AsyncSleep::eventQueue.front();
        AsyncSleep::eventQueue.pop();
        task();
    }
}

int main() {
    std::cout << "Main starts event loop\n";

    auto t1 = worker(1, 1000);
    auto t2 = worker(2, 500);

    t1.start();
    t2.start();

    // Simulate an event loop ticking
    while (true) {
        runEventLoop();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (AsyncSleep::eventQueue.empty()) break;
    }

    std::cout << "Main ends\n";
}
