#include "client.h"

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
