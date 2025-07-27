#pragma once
#include <cstring>
#include <unistd.h>

#pragma pack(push, 1)
template<typename ExchTraits>
struct MarketDataMessage {
    using seq_num_type = uint64_t;

    uint64_t sequence_number;
    uint32_t time_ms;

    char symbol[ExchTraits::SymbolSize +1];
    char exchange[ExchTraits::ExchangeSize+1];

    uint32_t bid_quantity;
    float    bid_price;

    float    offer_price;
    uint32_t offer_quantity;

    float    last_trade_price;
    float    close_price;

    MarketDataMessage(uint64_t seq) : sequence_number(seq) {
        std::memset(reinterpret_cast<char*>(this) + sizeof(seq_num_type), 0, sizeof(MarketDataMessage) - sizeof(seq_num_type));
    }
};
#pragma pack(pop)

template<typename ExchTraits>
class MarketDataMessageBuilder {

public:
    static MarketDataMessage<ExchTraits> build(uint64_t seq, const std::string& line) {
        MarketDataMessage<ExchTraits> msg(seq);
        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        msg.time_ms = std::stoul(token);

        std::getline(ss, token, ',');
        strncpy(msg.symbol, token.c_str(), sizeof(msg.symbol) - 1);

        std::getline(ss, token, ',');
        strncpy(msg.exchange, token.c_str(), sizeof(msg.exchange) - 1);

        std::getline(ss, token, ',');
        msg.bid_quantity = std::stoul(token);

        std::getline(ss, token, ',');
        msg.bid_price = std::stof(token);

        std::getline(ss, token, ',');
        msg.offer_price = std::stof(token);

        std::getline(ss, token, ',');
        msg.offer_quantity = std::stoul(token);

        std::getline(ss, token, ',');
        msg.last_trade_price = std::stof(token);

        std::getline(ss, token, ',');
        msg.close_price = std::stof(token);

        return msg;
    }
};
