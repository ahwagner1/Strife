#include "userRepo.hpp"

/*
    note to self about inheritance
    it doesn't seem like i can directly initilize m_conn from this function
    it throws error if i try to m_conn(connection) need to read up on this
    I have to basically init a BaseRepository object in this constructor it seems
*/
UserRepo::UserRepo(pqxx::connection* connection) :
    BaseRepository<User>(connection) {
}

void UserRepo::insert(const User& user) {
    const std::string query = 
        "INSERT INTO UserInfo (user_id, username, display_name, password) "
        "VALUES ($1, $2, $3, $4)";

    try {
        executeQuery(query, user.getId(), user.getUsername(),
                    user.getDisplayName(), user.getPassword());
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR, 
            "Failed to execute query: " + std::string(e.what()), 
            __PRETTY_FUNCTION__);
        
        return;
    }
}


std::optional<User> UserRepo::getById(const std::string& id) {
    const std::string query = 
        "SELECT user_id, username, display_name, password "
        "FROM UserInfo WHERE user_id = $1";

    pqxx::result result = executeQueryWithResult(query, id);

    if (result.empty()) {
        Logger::getInstance().log(Logger::WARNING, "No user matched given ID: " + id, __PRETTY_FUNCTION__);
        return std::nullopt;
    }

    const auto& row = result[0];
    return User(row["user_id"].as<std::string>(),
                row["username"].as<std::string>(),
                row["display_name"].as<std::string>(),
                row["password"].as<std::string>());
}

std::vector<User> UserRepo::getAll() {
    std::vector<User> users;

    const std::string query = "SELECT user_id, username, display_name, password FROM UserInfo";

    pqxx::result result = executeQueryWithResult(query);

    for (const auto& row : result) {
        users.emplace_back(
            row["user_id"].as<std::string>(),
            row["username"].as<std::string>(),
            row["display_name"].as<std::string>(),
            row["password"].as<std::string>()
        );
    }

    return users;
}

bool UserRepo::update(const User& user) {
    const std::string query = 
        "UPDATE UserInfo SET username = $2, display_name = $3, password = $4 "
        "WHERE user_id = $1";
    
    try {
        executeQuery(query, user.getId(), user.getUsername(), user.getDisplayName(), user.getPassword());
        return true;
    }
    catch (...) {
        Logger::getInstance().log(Logger::ERROR, "Failed to update user: " + user.getId(), __PRETTY_FUNCTION__);
        return false;
    }
}

bool UserRepo::remove(const std::string& id) {
    const std::string query = "DELETE FROM userInfo WHERE user_id = $1";
    
    try {
        executeQuery(query, id);
        return true;
    }
    catch (...) {
        Logger::getInstance().log(Logger::ERROR, "Failed to delet user: " + id, __PRETTY_FUNCTION__);
        return false;
    }
}

std::optional<User> UserRepo::getByUsername(const std::string& username) {
    const std::string query = 
        "SELECT user_id, username, display_name, password "
        "FROM UserInfo WHERE username = $1";

    pqxx::result result = executeQueryWithResult(query, username);
    
    if (result.empty()) {
        Logger::getInstance().log(Logger::WARNING, "No user with username: " + username, __PRETTY_FUNCTION__);
        return std::nullopt;
    }

    const auto& row = result[0];
    return User(
        row["user_id"].as<std::string>(),
        row["username"].as<std::string>(),
        row["display_name"].as<std::string>(),
        row["password"].as<std::string>()
    );
}