#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#pragma pack(push, 1)
struct MarketDataMessage {
    uint64_t sequence_number;
    uint32_t time_ms;

    char symbol[8];
    char exchange[8];

    uint32_t bid_quantity;
    float    bid_price;

    float    offer_price;
    uint32_t offer_quantity;

    float    last_trade_price;
    float    close_price;

    MarketDataMessage() {
        std::memset(this, 0, sizeof(MarketDataMessage));
    }

    MarketDataMessage(uint64_t seq, const std::string& line) : sequence_number(seq) {
        std::memset(this, 0, sizeof(MarketDataMessage));
        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        time_ms = std::stoul(token);

        std::getline(ss, token, ',');
        strncpy(symbol, token.c_str(), sizeof(symbol) - 1);

        std::getline(ss, token, ',');
        strncpy(exchange, token.c_str(), sizeof(exchange) - 1);

        std::getline(ss, token, ',');
        bid_quantity = std::stoul(token);

        std::getline(ss, token, ',');
        bid_price = std::stof(token);

        std::getline(ss, token, ',');
        offer_price = std::stof(token);

        std::getline(ss, token, ',');
        offer_quantity = std::stoul(token);

        std::getline(ss, token, ',');
        last_trade_price = std::stof(token);

        std::getline(ss, token, ',');
        close_price = std::stof(token);
    }
};
#pragma pack(pop)

int main() {
    const char* filename = "StockData.csv";
    const char* multicast_ip = "239.255.0.1";
    const int port = 30001;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << "\n";
        return 1;
    }

    // Skip header
    std::string line;
    std::getline(file, line);

    // Set up UDP multicast socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, multicast_ip, &addr.sin_addr);

    // Send messages line-by-line
    uint64_t sequence = 1;
    auto start_time = std::chrono::steady_clock::now();

    while (std::getline(file, line)) {
        MarketDataMessage msg(sequence++, line);

        auto target_time = start_time + std::chrono::milliseconds(msg.time_ms);
        std::this_thread::sleep_until(target_time);

        ssize_t sent = sendto(sockfd, &msg, sizeof(msg), 0,
                              reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        if (sent < 0) {
            perror("sendto failed");
        } else {
            std::cout << "Sent seq=" << msg.sequence_number << " symbol=" << msg.symbol << "\n";
        }
    }

    close(sockfd);
    return 0;
}
