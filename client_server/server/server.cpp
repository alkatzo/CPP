#include <iostream>
#include <thread>
#include <asio.hpp>
#include <vector>   // For std::vector

void handle_client(asio::ip::tcp::socket socket) {
    try {
        std::cout << "Client connected: " << socket.remote_endpoint() << std::endl;

        // First, read the length of the incoming message (4 bytes for uint32_t)
        uint32_t msg_length = 0;
        asio::read(socket, asio::buffer(&msg_length, sizeof(msg_length)));  // Read message size
        msg_length = ntohl(msg_length);  // Convert the size from network byte order

        std::cout << "Expected message size: " << msg_length << " bytes" << std::endl;

        // Now, read the actual message of the specified length
        std::vector<char> buffer(msg_length);
        asio::read(socket, asio::buffer(buffer, msg_length));  // Read the message

        std::string message(buffer.begin(), buffer.end());
        std::cout << "Received: " << message << std::endl;

        // Optionally, send a response back to the client
        std::string response = "Message received";
        asio::write(socket, asio::buffer(response));
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

        std::cout << "Server listening on port 8080..." << std::endl;

        while (true) {
            asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread(handle_client, std::move(socket)).detach();  // Handle each client in a new thread
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
