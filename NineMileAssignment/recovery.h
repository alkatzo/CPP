#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <future>

#include "exchange_traits.h"
#include "market_data_message.h"

#pragma pack(push, 1)
struct RecoveryRequest {
    uint64_t from_seq;
    uint64_t to_seq;
};
#pragma pack(pop)

template<typename ExchangeTraits>
class Recovery {
public:
    using MessageT = MarketDataMessage<ExchangeTraits>;
    using MessageBuilderT = MarketDataMessageBuilder<ExchangeTraits>;

    Recovery(uint16_t port, const std::string& filename)
        : port(port), filename(filename) {}

    void start(std::promise<void>& ready_promise) {
        std::thread([this, &ready_promise] { this->run(ready_promise); }).detach();
    }

private:
    uint16_t port;
    std::string filename;

    void run(std::promise<void>& ready_promise) {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("[Recovery] Recovery socket");
            return;
        }

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("[Recovery] Recovery bind");
            close(server_fd);
            return;
        }

        if (listen(server_fd, 5) < 0) {
            perror("[Recovery] Recovery listen");
            close(server_fd);
            return;
        }

        std::cout << "[Recovery] Listening on port " << port << std::endl;
        ready_promise.set_value();

        while (true) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
            if (client_fd >= 0) {
                std::thread(&Recovery::handle_client, this, client_fd).detach();
            } else {
                perror("[Recovery] Recovery accept");
            }
        }

        close(server_fd);
    }

    void handle_client(int client_fd) {
        RecoveryRequest req;
        ssize_t bytes_read = recv(client_fd, &req, sizeof(req), MSG_WAITALL);
        if (bytes_read != sizeof(req)) {
            std::cerr << "[Recovery] Invalid recovery request.\n";
            close(client_fd);
            return;
        }

        std::cout << "[Recovery] Client requested seq " << req.from_seq << " to " << req.to_seq << std::endl;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[Recovery] Could not open file: " << filename << std::endl;
            close(client_fd);
            return;
        }

        std::string line;
        std::getline(file, line); // skip header

        uint64_t current_seq = 1;

        while (std::getline(file, line)) {
            if (current_seq >= req.from_seq && current_seq <= req.to_seq) {
                MessageT msg = MessageBuilderT::build(current_seq, line);

                ssize_t sent = send(client_fd, &msg, sizeof(MessageT), 0);
                if (sent < 0) {
                    perror("[Recovery] send failed");
                    break;
                }
            }
            if (current_seq > req.to_seq) break;
            ++current_seq;
        }

        std::cout << "[Recovery] Done serving client\n";
        close(client_fd);
    }
};
