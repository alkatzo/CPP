#pragma once

#include <queue>
#include <map>
#include <vector>
#include <type_traits>
#include <thread>
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
    lock_strategy.lock();
    queue.push(data);
    statistics[data.symbol].messages_sent++;
    lock_strategy.get_condition().notify_one();
    lock_strategy.unlock();
}

template<typename LockStrategy>
bool SynchronizedQueue<LockStrategy>::try_pop(MarketData& data) {
    // For strategies that support condition variables (like MutexLockingStrategy)
    if constexpr (std::is_same_v<LockStrategy, MutexLockingStrategy>) {
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
    } else {
        // For other strategies, use polling approach
        while (true) {
            lock_strategy.lock();
            
            if (!queue.empty() || shutdown) {
                if (shutdown && queue.empty()) {
                    lock_strategy.unlock();
                    return false;
                }
                
                data = queue.top();
                queue.pop();
                
                // Update statistics
                statistics[data.symbol].update_stats(data);
                
                lock_strategy.unlock();
                return true;
            }
            
            lock_strategy.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

template<typename LockStrategy>
bool SynchronizedQueue<LockStrategy>::empty() const {
    lock_strategy.lock();
    bool result = queue.empty();
    lock_strategy.unlock();
    return result;
}

template<typename LockStrategy>
size_t SynchronizedQueue<LockStrategy>::size() const {
    lock_strategy.lock();
    size_t result = queue.size();
    lock_strategy.unlock();
    return result;
}

template<typename LockStrategy>
std::map<std::string, FeedStatisticsSnapshot> SynchronizedQueue<LockStrategy>::get_statistics() const {
    lock_strategy.lock();
    std::map<std::string, FeedStatisticsSnapshot> snapshot;
    for (const auto& [symbol, stat] : statistics) {
        snapshot.insert({symbol, FeedStatisticsSnapshot(stat)});
    }
    lock_strategy.unlock();
    return snapshot;
}

template<typename LockStrategy>
void SynchronizedQueue<LockStrategy>::shutdown_queue() {
    lock_strategy.lock();
    shutdown = true;
    lock_strategy.get_condition().notify_all();
    lock_strategy.unlock();
} 