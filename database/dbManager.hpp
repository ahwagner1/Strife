#ifndef STRIFE_DATABASE_DBHELPER_H
#define STRIFE_DATABASE_DBHELPER_H

/*
    Gonna use PostgreSQL for the database
    I'm gonna use libpqxx to connect to the database

    AS OF 3/30 @ 1500 AKST this should be working
    TODO:
        NEED TO DESIGN THE OBJECTS TO WORK ON VARIOUS TABLES
        PROBABLY WILL USE TEMPLATES INSTEAD OF A SINGLE CLASS FOR EVERY TABLE
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

#include <pqxx/pqxx>

#include "util/logger.hpp"

class DatabaseManager {

public:

    DatabaseManager(const std::string& dbName, const std::string& user, const std::string& password);
    ~DatabaseManager();
    bool connect();
    void _initDatabaseTables(); /* onetime use really, not needed for the long term */
    const pqxx::connection& getConnection() const;
    
private:
    std::string m_dbName;
    std::string m_user;
    std::string m_password;
    std::unique_ptr<pqxx::connection> m_conn;

    std::string createConnectionString();
};

#endif /* STRIFE_DATABASE_DBHELPER_H */