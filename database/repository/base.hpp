#ifndef STRIFE_REPOSITORY_BASE_H
#define STRIFE_REPOSITORY_BASE_H

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <pqxx/pqxx>

#include "database/dbManager.hpp"
#include "util/logger.hpp"

/*
    THIS CLASS HOLD THE BASE CLASS FOR THE TABLE CRUD REPOSITORY
    TABLE SPECIFIC CLASSES DEFINED IN specific.hpp WILL INHERIT FROM THIS CLASS
*/

template <typename Entity>
class BaseRepository {

public:
    BaseRepository(pqxx::connection* connection);
    virtual ~BaseRepository();

    virtual void insert(const Entity& entity) = 0;
    virtual std::optional<Entity> getById(const std::string& id) = 0;
    virtual std::vector<Entity> getAll() = 0;
    virtual bool update(const Entity& entity) = 0;
    virtual bool remove(const std::string& id) = 0;

protected: 
    pqxx::connection* m_conn;

    template<typename... Args>
    void executeQuery(const std::string& query, Args&&... args);

    template<typename... Args>
    pqxx::result executeQueryWithResult(const std::string& query, Args&&... args);
};



#endif /* STRIFE_REPOSITORY_BASE_H */