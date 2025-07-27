#include "MarketData.h"

MarketData::MarketData(const std::string& sym, double p, int vol, std::chrono::system_clock::time_point ts, int id)
    : symbol(sym), price(p), volume(vol), timestamp(ts), feed_id(id) {}

bool MarketData::operator>(const MarketData& other) const {
    return timestamp > other.timestamp;
} 