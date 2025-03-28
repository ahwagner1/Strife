#ifndef STRIFE_UTIL_LOGGER_H
#define STRIFE_UTIL_LOGGER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <mutex>


class Logger {

public:
    enum LogLevel {INFO, WARNING, ERROR};

    Logger(const std::string& filename);
    ~Logger();
    void log(LogLevel level, const std::string& message);

private:
    std::ofstream m_logFile;
    std::mutex m_logMutex;

    std::string currentDateTime();
    std::string logLevelToString(LogLevel level);
};

#endif /* STRIFE_UTIL_LOGGER_H */