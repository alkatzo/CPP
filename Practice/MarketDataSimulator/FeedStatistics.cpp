#include "FeedStatistics.h"

void FeedStatistics::update_stats(const MarketData& data) {
    messages_processed++;
    total_volume += data.volume;
    
    double current_min = min_price.load();
    while (data.price < current_min && 
           !min_price.compare_exchange_weak(current_min, data.price)) {}
    
    double current_max = max_price.load();
    while (data.price > current_max && 
           !max_price.compare_exchange_weak(current_max, data.price)) {}
    
    last_update = data.timestamp;
}

FeedStatisticsSnapshot::FeedStatisticsSnapshot(const FeedStatistics& stats) 
    : messages_sent(stats.messages_sent.load())
    , messages_processed(stats.messages_processed.load())
    , total_volume(stats.total_volume.load())
    , min_price(stats.min_price.load())
    , max_price(stats.max_price.load())
    , last_update(stats.last_update) {} 