#ifndef STRIFE_ENTITY_SERVER_H
#define STRIFE_ENTITY_SERVER_H

#include <string>

#include "util/logger.hpp"

class Server {

public:
	Server() = default;
	Server(const std::string& serverId, const std::string& serverName);

	const std::string& getId const;
	const std::string& getServerName const;

	void setId(const std::string& id);
	void setServerName(const std::string& serverName);

private:
	std::string m_serverId;
	std::string m_serverName;
};


#endif /* STRIFE_ENTITY_SERVER_H */