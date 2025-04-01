#ifndef STRIFE_REPOSITORY_SERVER_REPO_H
#define STRIFE_REPOSITORY_SERVER_REPO_H

#include <string>
#include <pqxx/pqxx>
#include <optional>

#include "util/logger.hpp"
#include "entity/user.hpp"
#include "base.hpp"

class ServerRepo : public BaseRepository<Server> {

public:
	ServerRepo(pqxx::connection* connection);

	void insert(const Server& server) override;
	std::optional<Server> getById(const std::string& id) override;
	std::vector<Server> getAll() override;
	bool update(const Server& server) override;
	bool remove(const Server& server) override;


};

#endif /* STRIFE_REPOSITORY_SERVER_REPO_H */