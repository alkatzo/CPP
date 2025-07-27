#pragma once

#include <string>
#include <chrono>

// Market data structure
struct MarketData {
    std::string symbol;
    double price;
    int volume;
    std::chrono::system_clock::time_point timestamp;
    int feed_id;
    
    // Constructors
    MarketData() = default;
    MarketData(const std::string& sym, double p, int vol, std::chrono::system_clock::time_point ts, int id);
    MarketData(const MarketData& other) = default;
    MarketData(MarketData&& other) noexcept = default;
    
    // Assignment operators
    MarketData& operator=(const MarketData& other) = default;
    MarketData& operator=(MarketData&& other) noexcept = default;
    
    // Destructor
    ~MarketData() = default;
    
    // For priority queue ordering (earliest timestamp first)
    bool operator>(const MarketData& other) const;
}; 