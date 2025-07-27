#include "MarketDataFeed.h"
#include <chrono>
#include <thread>

template<typename LockStrategy>
MarketDataFeed<LockStrategy>::MarketDataFeed(const std::string& sym, double base, double vol, SynchronizedQueue<LockStrategy>& q, int id)
    : symbol(sym), basePrice(base), volatility(vol), queue(q), gen(rd()), 
      price_dist(0.0, vol), volume_dist(100, 1000), delay_dist(100, 500), feed_id(id) {}

template<typename LockStrategy>
MarketDataFeed<LockStrategy>::~MarketDataFeed() {
    stop();
}

template<typename LockStrategy>
void MarketDataFeed<LockStrategy>::start() {
    running = true;
    feed_thread = std::thread(&MarketDataFeed<LockStrategy>::feed_loop, this);
}

template<typename LockStrategy>
void MarketDataFeed<LockStrategy>::stop() {
    running = false;
    if (feed_thread.joinable()) {
        feed_thread.join();
    }
}

template<typename LockStrategy>
std::string MarketDataFeed<LockStrategy>::get_symbol() const { 
    return symbol; 
}

template<typename LockStrategy>
void MarketDataFeed<LockStrategy>::feed_loop() {
    while (running) {
        // Generate random price movement
        double price_change = price_dist(gen);
        basePrice += price_change;
        
        // Ensure price doesn't go negative
        if (basePrice < 0.01) basePrice = 0.01;
        
        // Generate random volume
        int volume = volume_dist(gen);
        
        // Create market data with current timestamp
        auto timestamp = std::chrono::system_clock::now();
        MarketData data(symbol, basePrice, volume, timestamp, feed_id);
        
        // Add to synchronized queue
        queue.push(data);
        
        // Random delay between updates
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_dist(gen)));
    }
} 