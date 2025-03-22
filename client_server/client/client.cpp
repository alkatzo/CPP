#include <iostream>
#include <asio.hpp>
#include <cstring>  // For std::strlen

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::socket socket(io_context);
        socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 8080));

        std::cout << "Connected to server\n";

        std::string message = "Hello from client!";
        uint32_t msg_length = htonl(message.size()); // Convert message size to network byte order

        // First, send the length of the message
        asio::write(socket, asio::buffer(&msg_length, sizeof(msg_length)));

        // Then send the actual message
        asio::write(socket, asio::buffer(message));

        std::cout << "Sent: " << message << std::endl;

        // Read the response from the server (optional)
        char buffer[1024];
        std::size_t bytes_received = socket.read_some(asio::buffer(buffer));
        std::cout << "Received from server: " << std::string(buffer, bytes_received) << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
