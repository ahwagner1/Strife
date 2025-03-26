#ifndef STRIFE_NETWORKING_SERVER_H
#define STRIFE_NETWORKING_SERVER_H

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <functional>
#include <atomic>
#include <cstring>

// for cross platform socket support (thank you claude)
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET socket_t;
    #define SOCKET_ERROR_VALUE INVALID_SOCKET
    inline int closeSocket(socket_t sock) { return closesocket(sock); }
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <arpa/inet.h>
    typedef int socket_t;
    #define SOCKET_ERROR_VALUE (-1)
    inline int closeSocket(socket_t sock) { return close(sock); }
#endif

class TcpServer {

public:
    // MessageHandler is a placeholder for now until I can create the message protocol
    using MessageHandler = std::function<void(int clientId, const std::string& message)>;

    TcpServer(int port, MessageHandler handler);
    ~TcpServer();
    bool start();
    void stop();
    bool sendToClient(int clientId, const std::string& message);
    void broadcastMessage(const std::string& message, int excludeClientId);

private:

    struct Client {
        int id;
        socket_t socket;
        std::thread thread;
        bool running;
    };

    int m_port;
    socket_t m_serverSocket;
    std::thread m_acceptThread;
    std::vector<Client> m_clients;
    std::mutex m_clientsMutex; 
    std::atomic<bool> m_running;
    std::atomic<int> m_nextClientId;
    MessageHandler m_messageHandler;

    void acceptClients();
    void handleClient(Client& client);
};

#endif /* STRIFE_NETWORKING_SERVER_H*/