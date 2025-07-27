#include <iostream>
#include <vector>
#include <memory>
#include "SynchronizedQueue.h"
#include "MarketDataFeed.h"
#include "MarketDataReader.h"
#include "LockingStrategy.h"

int main() {
    std::cout << "Enhanced Market Data Simulator Starting..." << std::endl;
    std::cout << "Features: 3 feeds, synchronized queue, timestamp ordering, statistics" << std::endl;
    std::cout << "Using MutexLockingStrategy by default" << std::endl;
    
    // Create synchronized queue with default mutex locking strategy
    SynchronizedQueue<MutexLockingStrategy> market_queue;
    
    // Create 3 market data feeds for different symbols
    std::vector<std::unique_ptr<MarketDataFeed<MutexLockingStrategy>>> feeds;
    feeds.push_back(std::make_unique<MarketDataFeed<MutexLockingStrategy>>("AAPL", 150.0, 0.5, market_queue, 1));
    feeds.push_back(std::make_unique<MarketDataFeed<MutexLockingStrategy>>("GOOGL", 2800.0, 2.0, market_queue, 2));
    feeds.push_back(std::make_unique<MarketDataFeed<MutexLockingStrategy>>("TSLA", 800.0, 5.0, market_queue, 3));
    
    // Create market data reader
    MarketDataReader<MutexLockingStrategy> reader(market_queue);
    
    // Start all feeds and reader
    for (auto& feed : feeds) {
        feed->start();
    }
    reader.start();
    
    std::cout << "Press Enter to stop the simulation..." << std::endl;
    std::cin.get();
    
    // Stop all components
    for (auto& feed : feeds) {
        feed->stop();
    }
    reader.stop();
    
    // Final statistics
    std::cout << "\n=== FINAL STATISTICS ===" << std::endl;
    std::cout << "Total messages processed: " << reader.get_messages_read() << std::endl;
    
    auto final_stats = market_queue.get_statistics();
    for (const auto& [symbol, stat] : final_stats) {
        std::cout << symbol << " - Sent: " << stat.messages_sent 
                  << ", Processed: " << stat.messages_processed
                  << ", Total Volume: " << stat.total_volume
                  << ", Price Range: $" << stat.min_price << " - $" << stat.max_price << std::endl;
    }
    
    std::cout << "Enhanced Market Data Simulator stopped." << std::endl;
    return 0;
} 