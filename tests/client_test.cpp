#include "networking/client.hpp"

int main() {

    TcpClient client("127.0.0.1", 8080, [](const std::string& message) {
        std::cout << "Recieved: " << message << std::endl;
    });

    if (!client.connect()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    std::string message;
    while (client.isConnected()) {
        std::cout << "Enter message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") {
            break;
        }

        if (!client.sendMessage(message)) {
            std::cerr << "Failed to send message" <<std::endl;
            break;
        }
    }

    client.disconnect();
    return 0;
}