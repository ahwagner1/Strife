#include "base.hpp"

template <typename Entity>
BaseRepository<Entity>::BaseRepository(pqxx::connection* connection) :
    m_conn(connection) {
}

template <typename Entity>
BaseRepository<Entity>::~BaseRepository() = default;

template <typename Entity>
template<typename... Args>
void BaseRepository<Entity>::executeQuery(const std::string& query, Args&&... args) {
    if (!m_conn) {
        Logger::getInstance().log(Logger::ERROR, "Conncetions to database not established", __PRETTY_FUNCTION__);
        return;
    }

    try {
        pqxx::work txn(*m_conn);
        tx.exec_params(query, std::forward<Args>(args)...);
        txn.commit();
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR, "Failed to execute query" + std::string(e.what()), __PRETTY_FUNCTION__);
        throw;
    }
}

template <typename Entity>
template<typename... Args>
pqxx::result BaseRepository<Entity>::executeQueryWithResult(const std::string& query, Args&&... args) {
    if (!m_conn) {
        Logger::getInstance().log(Logger::ERROR, "Conncetions to database not established", __PRETTY_FUNCTION__);
        return;
    }

    try {
        pqxx::work txn(*m_conn);
        pqxx::result r = tx.exec_params(query, std::forward<Args>(args)...);
        txn.commit();
        return r;
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR, "Failed to execute query" + std::string(e.what()), __PRETTY_FUNCTION__);
        throw;
    }
}