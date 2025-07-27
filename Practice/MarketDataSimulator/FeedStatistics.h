#pragma once

#include <atomic>
#include <chrono>
#include <limits>
#include "MarketData.h"

// Statistics tracking
struct FeedStatistics {
    std::atomic<int> messages_sent{0};
    std::atomic<int> messages_processed{0};
    std::atomic<double> total_volume{0.0};
    std::atomic<double> min_price{std::numeric_limits<double>::max()};
    std::atomic<double> max_price{std::numeric_limits<double>::lowest()};
    std::chrono::system_clock::time_point last_update;
    
    // Constructors
    FeedStatistics() = default;
    FeedStatistics(const FeedStatistics&) = delete;  // atomic types are not copyable
    FeedStatistics(FeedStatistics&&) = delete;       // atomic types are not movable
    
    // Assignment operators
    FeedStatistics& operator=(const FeedStatistics&) = delete;  // atomic types are not copyable
    FeedStatistics& operator=(FeedStatistics&&) = delete;       // atomic types are not movable
    
    // Destructor
    ~FeedStatistics() = default;
    
    void update_stats(const MarketData& data);
};

// Non-atomic version for returning statistics
struct FeedStatisticsSnapshot {
    int messages_sent;
    int messages_processed;
    double total_volume;
    double min_price;
    double max_price;
    std::chrono::system_clock::time_point last_update;
    
    // Constructors
    FeedStatisticsSnapshot() = default;
    FeedStatisticsSnapshot(const FeedStatistics& stats);
    FeedStatisticsSnapshot(const FeedStatisticsSnapshot& other) = default;
    FeedStatisticsSnapshot(FeedStatisticsSnapshot&& other) noexcept = default;
    
    // Assignment operators
    FeedStatisticsSnapshot& operator=(const FeedStatisticsSnapshot& other) = default;
    FeedStatisticsSnapshot& operator=(FeedStatisticsSnapshot&& other) noexcept = default;
    
    // Destructor
    ~FeedStatisticsSnapshot() = default;
}; 