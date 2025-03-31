#include "dbManager.hpp"

DatabaseManager::DatabaseManager(const std::string& dbName, const std::string& user, const std::string& password) :
    m_dbName(dbName),
    m_user(user),
    m_password(password) {
}

DatabaseManager::~DatabaseManager() {
}

bool DatabaseManager::connect() {
    std::string connectionString = createConnectionString();
    
    try {
        m_conn = std::make_unique<pqxx::connection>(connectionString);
        return true;
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR,
            std::string("Connection Failed: ") + e.what(), __PRETTY_FUNCTION__);
        return false;
    }
}

std::string DatabaseManager::createConnectionString() {
    std::string connectionString = "host=localhost port=5432 dbname=" + m_dbName + " user=" + m_user + " password=" + m_password; 
    Logger::getInstance().log(Logger::INFO, connectionString, __PRETTY_FUNCTION__);
    return connectionString;
}

void DatabaseManager::_initDatabaseTables() {
    pqxx::work tx(*m_conn);

    std::vector<std::string> commands;

    std::ifstream file("/home/ahwagner/repos/Strife/database/init_table_strings.txt");
    if (!file.is_open()) {
        Logger::getInstance().log(Logger::ERROR, "Failed to open SQL Table Command File", __PRETTY_FUNCTION__);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        Logger::getInstance().log(Logger::INFO, line, __PRETTY_FUNCTION__);
        commands.push_back(line);
    }
    file.close();

    try {
        for (std::string& cmd : commands) {
            pqxx::result r = tx.exec(cmd);
            tx.commit();
        }
    }
    catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR, 
            std::string("Failed to create tables") + e.what(), __PRETTY_FUNCTION__);
        return;
    }
}

const pqxx::connection& DatabaseManager::getConnection() const {
    if (!m_conn) {
        throw std::runtime_error("Database not connected");
    }
    
    return *m_conn;
}

// just used for testing the logger and reading the file
/*int main() {
    DatabaseManager dbMan("db", "ahwagner", "1234");
    dbMan._initDatabaseTables();

    return 0;
}*/