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

#include "exchange_traits.h"
#include "market_data_message.h"
#include "recovery.h"

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Usage : cmd <path to csv input file>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const char* multicast_ip = "239.255.0.1";
    const int port = 30001;
    const int recovery_port = 30002;

    using ExchangeTraits = ExTraits<3,4>;
    using MessageT = MarketDataMessage<ExchangeTraits>;
    using MessageBuilderT = MarketDataMessageBuilder<ExchangeTraits>;
    using RecoveryT = Recovery<ExchangeTraits>;

    // std::cout << sizeof(decltype(std::declval<MessageBuilderT>().build(0, std::string()))) << std::endl;

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

    // Set up TCP recovery mechanism
    RecoveryT recovery(recovery_port, filename);
    std::promise<void> recovery_ready;
    std::future<void> recovery_future = recovery_ready.get_future();
    recovery.start(recovery_ready);

    // Wait for recovery server to start
    recovery_future.get();

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, multicast_ip, &addr.sin_addr);

    // Send messages line-by-line
    uint64_t sequence = 1;
    auto start_time = std::chrono::steady_clock::now();

    while (std::getline(file, line)) {
        MessageT msg = MessageBuilderT::build(sequence++, line);

        auto target_time = start_time + std::chrono::milliseconds(msg.time_ms);
        std::this_thread::sleep_until(target_time);

        ssize_t sent = sendto(sockfd, &msg, sizeof(msg), 0,
                              reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        if (sent < 0) {
            perror("sendto failed");
        } else {
            std::cout << "Sent seq=" << msg.sequence_number << " symbol=" << msg.symbol << std::endl;
        }
    }

    close(sockfd);

    std::cout << "Done" << std::endl;


    return 0;
}
