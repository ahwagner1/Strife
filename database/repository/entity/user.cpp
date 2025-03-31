#include "user.hpp"

User::User() = default;

User::User(const std::string& id, const std::string& username, const std::string& displayName, const std::string& password) :
    m_id(id),
    m_username(username),
    m_displayName(displayName),
    m_password(password) {
}

const std::string& User::getId() const {
    return m_id;
}

const std::string& User::getUsername() const {
    return m_username;
}

const std::string& User::getDisplayName() const {
    return m_displayName;
}

const std::string& User::getPassword() const {
    return m_password;
}

void User::setUsername(const std::string& username) {
    m_username = username;
}

void User::setDisplayName(const std::string& displayName) {
    m_displayName = displayName;
}

void User::setPassword(const std::string& password) {
    m_password = password;
    Logger::getInstance().log(Logger::WARNING, "Password updated, need to update database", __PRETTY_FUNCTION__);
    // IF THIS GETS UPDATED, NEED TO UPDATE THE DATABASE TOO
}