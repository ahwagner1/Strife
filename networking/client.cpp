#include "client.hpp"

TcpClient::TcpClient(const std::string& serverIp, int serverPort, MessageHandler handler) : 
    m_serverIp(serverIp),
    m_serverPort(serverPort),
    m_socket(SOCKET_ERROR_VALUE),
    m_connected(false),
    m_running(false),
    m_messageHandler(handler) {
}

TcpClient::~TcpClient() {
    disconnect();
}

bool TcpClient::connect() {
    // init socket library on windows
    #ifdef _WIN32
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to init Winsock" << std::endl;
        return false;
    }
    #endif

    // create our socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == SOCKET_ERROR_VALUE) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    // prepare the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);

    // convert the IP address string to binary form
    if (inet_pton(AF_INET, m_serverIp.c_str(), &(serverAddr.sin_addr)) <= 0) {
        std::cerr << "Invalid address: " << m_serverIp << std::endl;
        closeSocket(m_socket);
        m_socket = SOCKET_ERROR_VALUE;
        return false;
    }

    // connect to the server
    if (::connect(m_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Conneciton failed to " << m_serverIp << ":" << m_serverPort << std::endl;
        closeSocket(m_socket);
        m_socket = SOCKET_ERROR_VALUE;
        return false;
    }

    m_connected = true;
    m_running = true;

    // start message receiving thread
    m_receiveThread = std::thread(&TcpClient::receieveMessages, this);

    std::cout << "Connected to server " << m_serverIp << ":" << m_serverPort << std::endl;
    return true;
}

void TcpClient::disconnect() {
    if (!m_connected) return;

    m_connected = false;
    m_running = false;
    
    if (m_socket != SOCKET_ERROR_VALUE) {
        closeSocket(m_socket);
        m_socket = SOCKET_ERROR_VALUE;
    }

    if (m_receiveThread.joinable()) {
        m_receiveThread.join();
    }

    #ifdef _WIN32
    WSACleanup();
    #endif

    std::cout << "Disconnected from server" << std::endl;
}

bool TcpClient::sendMessage(const std::string& message) {
    if (!m_connected || m_socket == SOCKET_ERROR_VALUE) {
        std::cerr << "Cannot send message: not connected tpo server" << std::endl;
        return false;
    }

    uint32_t length = htonl(static_cast<uint32_t>(message.length()));

    // send elnght first
    if (send(m_socket, reinterpret_cast<char*>(&length), sizeof(length), 0) < 0) {
        std::cerr << "Failed to send message length" << std::endl;
        return false;
    }

    // send actual message
    if (send(m_socket, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return false;
    }

    return true;
}

bool TcpClient::isConnected() const {
    return m_connected;
}

void TcpClient::receieveMessages() {
    const int BUFFER_SIZE = 4096; // may tweak this in the future
    char buffer[BUFFER_SIZE];

    while (m_running) {
        uint32_t messageLength = 0;
        int bytesRead = recv(m_socket, reinterpret_cast<char*>(&messageLength), sizeof(messageLength), 0);

        if (bytesRead <= 0) {
            break; // connection closed or error
        }

        messageLength = ntohl(messageLength);
        if (messageLength > BUFFER_SIZE) {
            std::cerr << "Message too large for server" << std::endl;
            break;
        }

        bytesRead = recv(m_socket, buffer, messageLength, 0);
        if (bytesRead <= 0) {
            break;
        }

        std::string message(buffer, bytesRead);
        if (m_messageHandler) {
            m_messageHandler(message);
        }
    }

    // if we break exited loop because of connection error but the client still thinks its connected
    if (m_connected) {
        m_connected = false;
        std::cerr << "Lost connection to server" << std::endl;
    }
}