#ifndef STRIFE_ENTITY_USER_H
#define STRIFE_ENTITY_USER_H

#include <string>

#include "util/logger.hpp"

class User {

public:
    User() = default;
    User(const std::string& id, const std::string& username, const std::string& displayName, const std::string& password);

    const std::string& getId() const; // need to find a uuid library or make one to create id's for people
    const std::string& getUsername() const;
    const std::string& getDisplayName() const;
    const std::string& getPassword() const; // NNEDS TO BE HASHED, ALSO LIKELY NEED TO UPDATE DB TO HAVE SALT AT THE VERY LEAST

    void setUsername(const std::string& username);
    void setDisplayName(const std::string& username);
    void setPassword(const std::string& username); // may or may not need this, idk?
    

private:
    std::string m_id;
    std::string m_username;
    std::string m_displayName;
    std::string m_password;

    // add salt and sha 256 password hashing functions here for anything related to a users password
};


#endif /* STRIFE_ENTITY_USER_H */