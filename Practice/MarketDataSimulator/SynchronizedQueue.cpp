#include "SynchronizedQueue.h"

void SynchronizedQueue::push(const MarketData& data) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(data);
    statistics[data.symbol].messages_sent++;
    condition.notify_one();
}

bool SynchronizedQueue::try_pop(MarketData& data) {
    std::unique_lock<std::mutex> lock(mutex);
    
    // Wait until there's data or shutdown is requested
    condition.wait(lock, [this] { return !queue.empty() || shutdown; });
    
    if (shutdown && queue.empty()) {
        return false;
    }
    
    data = queue.top();
    queue.pop();
    
    // Update statistics
    statistics[data.symbol].update_stats(data);
    
    return true;
}

bool SynchronizedQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
}

size_t SynchronizedQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.size();
}

std::map<std::string, FeedStatisticsSnapshot> SynchronizedQueue::get_statistics() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::map<std::string, FeedStatisticsSnapshot> snapshot;
    for (const auto& [symbol, stat] : statistics) {
        snapshot.insert({symbol, FeedStatisticsSnapshot(stat)});
    }
    return snapshot;
}

void SynchronizedQueue::shutdown_queue() {
    std::lock_guard<std::mutex> lock(mutex);
    shutdown = true;
    condition.notify_all();
} 