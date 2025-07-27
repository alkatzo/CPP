#include "MarketDataReader.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>

template<typename LockStrategy>
MarketDataReader<LockStrategy>::MarketDataReader(SynchronizedQueue<LockStrategy>& q) : queue(q) {}

template<typename LockStrategy>
MarketDataReader<LockStrategy>::~MarketDataReader() {
    stop();
}

template<typename LockStrategy>
void MarketDataReader<LockStrategy>::start() {
    running = true;
    reader_thread = std::thread(&MarketDataReader<LockStrategy>::read_loop, this);
    stats_thread = std::thread(&MarketDataReader<LockStrategy>::stats_loop, this);
}

template<typename LockStrategy>
void MarketDataReader<LockStrategy>::stop() {
    running = false;
    queue.shutdown_queue();
    if (reader_thread.joinable()) {
        reader_thread.join();
    }
    if (stats_thread.joinable()) {
        stats_thread.join();
    }
}

template<typename LockStrategy>
int MarketDataReader<LockStrategy>::get_messages_read() const { 
    return messages_read; 
}

template<typename LockStrategy>
void MarketDataReader<LockStrategy>::read_loop() {
    while (running) {
        MarketData data;
        if (queue.try_pop(data)) {
            messages_read++;
            
            // Convert timestamp to readable format
            auto time_t = std::chrono::system_clock::to_time_t(data.timestamp);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                data.timestamp.time_since_epoch()) % 1000;
            
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
            ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
            
            // Print the market data with feed ID
            std::cout << "[" << ss.str() << "] Feed-" << data.feed_id 
                      << " " << data.symbol << " - Price: $" 
                      << std::fixed << std::setprecision(2) << data.price
                      << " Volume: " << data.volume << std::endl;
        }
    }
}

template<typename LockStrategy>
void MarketDataReader<LockStrategy>::stats_loop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        if (!running) break;
        
        auto stats = queue.get_statistics();
        std::cout << "\n=== STATISTICS UPDATE ===" << std::endl;
        std::cout << "Messages read: " << messages_read << std::endl;
        std::cout << "Queue size: " << queue.size() << std::endl;
        
        for (const auto& [symbol, stat] : stats) {
            std::cout << symbol << " - Sent: " << stat.messages_sent 
                      << ", Processed: " << stat.messages_processed
                      << ", Total Volume: " << stat.total_volume
                      << ", Price Range: $" << stat.min_price << " - $" << stat.max_price << std::endl;
        }
        std::cout << "========================\n" << std::endl;
    }
} 