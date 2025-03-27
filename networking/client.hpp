#ifndef STRIFE_NETWORKING_CLIENT_H
#define STRIFE_NETWORKING_CLIENT_H

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
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

class TcpClient{

public:
    // MessageHandler is a placeholder for now until I can create the message protocol
    using MessageHandler = std::function<void(const std::string& message)>;

    TcpClient(const std::string& serverIp, int serverPort, MessageHandler handler);
    ~TcpClient();
    bool connect();
    void disconnect();
    bool sendMessage(const std::string& message);
    bool isConnected() const;

private:
    std::string m_serverIp;
    int m_serverPort;
    socket_t m_socket;
    std::atomic<bool> m_connected;
    std::atomic<bool> m_running;
    std::thread m_receiveThread;
    MessageHandler m_messageHandler;

    void receieveMessages();
};

#endif /* STRIFE_NETWORKING_CLIENT_H*/