#ifndef STRIFE_REPOSITORY_USER_REPO_H
#define STRIFE_REPOSITORY_USER_REPO_H

#include <string>
#include <pqxx/pqxx>
#include <optional>

#include "util/logger.hpp"
#include "entity/user.hpp"
#include "base.hpp"

class UserRepo : public BaseRepository<User> {

public:
    UserRepo(pqxx::connection* connection);

    void insert(const User& user) override;
    std::optional<User> getById(const std::string& id) override;
    std::vector<User> getAll() override;
    bool update(const User& user) override;
    bool remove(const std::string& id) override;

    std::optional<User> getByUsername(const std::string& username);
};

#endif /* STRIFE_REPOSITORY_USER_REPO_H */