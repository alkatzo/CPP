#pragma once
#include <string>
#include <thread>
#include <optional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace feed {

struct Instrument {
    std::string ticker;
    enum class Type {Stock, Future, Option} type;
};

struct PriceUpdate {
    float price;
    int volume;
    Instrument instrument;
};

template<typename T>
class SyncQueue {
public:
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        cond_var_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [&](){ return !queue_.empty(); }); // Wait until notified AND condition is rechecked
        T item = queue_.front();
        queue_.pop();
        return item;
    }

    bool try_pop(T& out) {
        std::unique_lock<std::mutex> lock(mutex_);
        if(queue_.empty())
            return false;

        out = queue_.front();
        queue_.pop();
        return true;
    }

    size_t size() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool empty() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};

class Feed
{
public:
    Feed();
    void start();
    void stop();

private:
    std::optional<std::thread> thread_opt;
    SyncQueue<PriceUpdate> queue_;
};

}
