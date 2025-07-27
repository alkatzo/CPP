#pragma once

#include <queue>
#include <map>
#include <vector>
#include "MarketData.h"
#include "FeedStatistics.h"
#include "LockingStrategy.h"
#include "LockGuard.h"

// Thread-safe synchronized queue with statistics
template<typename LockStrategy = MutexLockingStrategy>
class SynchronizedQueue {
private:
    std::priority_queue<MarketData, std::vector<MarketData>, std::greater<MarketData>> queue;
    mutable LockStrategy lock_strategy;
    bool shutdown = false;
    std::map<std::string, FeedStatistics> statistics;

public:
    // Constructors
    SynchronizedQueue() = default;
    SynchronizedQueue(const SynchronizedQueue&) = delete;  // locking strategy may not be copyable
    SynchronizedQueue(SynchronizedQueue&&) = delete;       // locking strategy may not be movable
    
    // Assignment operators
    SynchronizedQueue& operator=(const SynchronizedQueue&) = delete;  // locking strategy may not be copyable
    SynchronizedQueue& operator=(SynchronizedQueue&&) = delete;       // locking strategy may not be movable
    
    // Destructor
    ~SynchronizedQueue() = default;
    
    // Add market data to queue
    void push(const MarketData& data);
    
    // Try to pop the earliest timestamped data
    bool try_pop(MarketData& data);
    
    // Check if queue is empty
    bool empty() const;
    
    // Get queue size
    size_t size() const;
    
    // Get statistics
    std::map<std::string, FeedStatisticsSnapshot> get_statistics() const;
    
    // Shutdown the queue
    void shutdown_queue();
};

// Template implementation
template<typename LockStrategy>
void SynchronizedQueue<LockStrategy>::push(const MarketData& data) {
    std::lock_guard<std::mutex> lock(lock_strategy.get_mutex());
    queue.push(data);
    statistics[data.symbol].messages_sent++;
    lock_strategy.get_condition().notify_one();
}

template<typename LockStrategy>
bool SynchronizedQueue<LockStrategy>::try_pop(MarketData& data) {
    std::unique_lock<std::mutex> lock(lock_strategy.get_mutex());
    
    // Wait until there's data or shutdown is requested
    lock_strategy.get_condition().wait(lock, [this] { return !queue.empty() || shutdown; });
    
    if (shutdown && queue.empty()) {
        return false;
    }
    
    data = queue.top();
    queue.pop();
    
    // Update statistics
    statistics[data.symbol].update_stats(data);
    
    return true;
}

template<typename LockStrategy>
bool SynchronizedQueue<LockStrategy>::empty() const {
    std::lock_guard<std::mutex> lock(lock_strategy.get_mutex());
    return queue.empty();
}

template<typename LockStrategy>
size_t SynchronizedQueue<LockStrategy>::size() const {
    std::lock_guard<std::mutex> lock(lock_strategy.get_mutex());
    return queue.size();
}

template<typename LockStrategy>
std::map<std::string, FeedStatisticsSnapshot> SynchronizedQueue<LockStrategy>::get_statistics() const {
    std::lock_guard<std::mutex> lock(lock_strategy.get_mutex());
    std::map<std::string, FeedStatisticsSnapshot> snapshot;
    for (const auto& [symbol, stat] : statistics) {
        snapshot.insert({symbol, FeedStatisticsSnapshot(stat)});
    }
    return snapshot;
}

template<typename LockStrategy>
void SynchronizedQueue<LockStrategy>::shutdown_queue() {
    std::lock_guard<std::mutex> lock(lock_strategy.get_mutex());
    shutdown = true;
    lock_strategy.get_condition().notify_all();
} 