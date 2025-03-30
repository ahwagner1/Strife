#ifndef STRIFE_UTIL_LOGGER_H
#define STRIFE_UTIL_LOGGER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <mutex>

/*
    This is setup so there can only be a single instance of a logger, ever
    Something called a singleton class instance which is something new I learned
    Basically, on any object that I import logger.hpp, I can create logs via:
        Logger::getInstance().log(LOGGER::INFO/WARNING/ERROR, "INFO/WARNING/ERROR")

    the static method getInstance() mets the method be called without initializing an object
    and the static variable ensures that only one logger object is created
*/

class Logger {

public:
    enum LogLevel {INFO, WARNING, ERROR};

    ~Logger();
    static Logger& getInstance(const std::string& filename = "app.log");
    void log(LogLevel level, const std::string& message, const std::string& funcName);

private:
    std::ofstream m_logFile;
    std::mutex m_logMutex;

    Logger(const std::string& filename);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string currentDateTime();
    std::string logLevelToString(LogLevel level);
};

#endif /* STRIFE_UTIL_LOGGER_H */