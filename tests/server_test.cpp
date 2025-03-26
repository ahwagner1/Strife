#include "networking/server.h"

int main() {

    TcpServer server(8080, [&server](int clientId, const std::string& message) {
        std::cout << "Message from client " << clientId << ": " << message << std::endl;

        // echo message back to all clients
        server.broadcastMessage("Client " + std::to_string(clientId) + ": " + message);
    });

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Press enter to stop the server..." << std::endl;
    std::cin.get();

    server.stop();
    return 0;
}