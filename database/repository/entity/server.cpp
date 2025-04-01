#include "server.hpp"

Server::Server(const std::string& serverId, const std::string& serverName) :
	m_serverId(serverId),
	m_serverId(serverName) {
}

const std::string& Server::getId() const {
	return m_serverId;
}

const std::string& Server::getServerName() const {
	return m_serverName;
}

void Server::setId(const std::& string id) {
	m_serverID = id;
}

void Server::setServerName(const std::& string serverName) {
	m_serverName = serverName;
}