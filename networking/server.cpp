#include "server.h"

TcpServer::TcpServer(int port, MessageHandler handler) : 
    m_port(port),
    m_messageHandler(handler),
    m_running(false),
    m_nextClientId(0) {
}
    
TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::start() {
    // init socket library on Winfows
    #ifdef _WIN32
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to init Winsock" << std::endl;
        return false;
    }
    #endif

    // create socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == SOCKET_ERROR_VALUE) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    // enable address reuse
    int opt = 1;
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        return false;
    }

    // prepare server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(m_port);

    // bind socket
    if (bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket to port " << m_port << std::endl;
        closeSocket(m_serverSocket);
        return false;
    }

    // listen for incoming connections
    if (listen(m_serverSocket, 10) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        closeSocket(m_serverSocket);
        return false;
    }

    std::cout << "Server started on port " << m_port << std::endl;

    m_running = true;
    m_acceptThread = std::thread(&TcpServer::acceptClients, this);

    return true;
}

void TcpServer::stop() {
    if (!m_running) return;

    m_running = false;

    closeSocket(m_serverSocket);

    // join accept thread
    if (m_acceptThread.joinable()) {
        m_acceptThread.join();
    }

    // close all client connections
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (auto& client : m_clients) {
        closeSocket(client.socket);
        if (client.thread.joinable()) {
            client.thread.join();
        }
    }
    m_clients.clear();

    #ifdef _WIN32
    WSACleanup();
    #endif
}

bool TcpServer::sendToClient(int clientId, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    auto it = std::find_if(m_clients.begin(), m_clients.end(),
                            [clientId](const Client& client) {return client.id == clientId; });

    if (it == m_clients.end()) {
        return false;
    }

    // add message length prefix for proper messge framing
    uint32_t length = htonl(static_cast<uint32_t>(message.length()));

    // send the message length prefix first
    if (send(it->socket, reinterpret_cast<const char*>(&length), sizeof(length), 0) < 0) {
        return false;
    }

    // send actual messgae
    if (send(it->socket, message.c_str(), message.length(), 0) < 0) {
        return false;
    }

    return true;
}

void TcpServer::broadcastMessage(const std::string& message, int excludeClientId) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (const auto& client : m_clients) {
        if (client.id != excludeClientId) {
            sendToClient(client.id, message);
        }
    }
}

void TcpServer::acceptClients() {
    while (m_running) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        socket_t clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == SOCKET_ERROR_VALUE) {
            if (m_running) {
                std::cerr << "Failed to accept client connections" << std::endl;
            }
            break;
        }

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);

        int clientId = m_nextClientId++;
        std::cout << "Client connected: " << clientIp << " (ID: " << clientId << ")" << std::endl;

        // add new client
        Client newClient;
        newClient.id = clientId;
        newClient.socket = clientSocket;

        // std::ref is pretty neat here, the std::thread constructor makes copies of all arguements to store but our client object does not support copying since we have non-copyable members (mutex)
        newClient.thread = std::thread(&TcpServer::handleClient, this, std::ref(newClient));
        newClient.running = true;

        // add client to list
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients.push_back(std::move(newClient));
        }
    }
}

void TcpServer::handleClient(Client& client) {
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    int clientId = client.id;
    socket_t clientSocket = client.socket;

    while (m_running && client.running) {
        // first read the message length (4 bytes)
        uint32_t messageLength = 0;
        int bytesRead = recv(clientSocket, reinterpret_cast<char*>(&messageLength), sizeof(messageLength), 0);

        if (bytesRead <= 0) {
            break; // client disconnected or error
        }

        messageLength = ntohl(messageLength);
        if (messageLength > BUFFER_SIZE) {
            std::cerr << "Message too large from client " << clientId << std::endl;
            break;
        }

        // read actual message
        bytesRead = recv(clientSocket, buffer, messageLength, 0);
        if (bytesRead <= 0) {
            break;
        }

        //convert to string and handle
        std::string message(buffer, bytesRead);
        if (m_messageHandler) {
            continue;
            //m_messageHandler(clientId, &message); // this doesn't really do anything right now
        }
    }

    // client disconncted
    std::cout << "Client disconnected (ID: " << clientId << ")" << std::endl;
    closeSocket(clientSocket);

    // remove client from list
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        m_clients.erase(
            std::remove_if(m_clients.begin(), m_clients.end(),
                            [clientId](const Client& c) {return c.id == clientId; }),
                            m_clients.end()
        );
    }
}