#include "logger.hpp"

Logger::Logger(const std::string& filename) : m_logFile(filename, std::ios::app) {
    if (!m_logFile.is_open()) {
        std::cerr << "Unable to open log file" << std::endl;
    }
}

Logger::~Logger() {
    std::lock_guard<std::mutex> guard(m_logMutex);
    m_logFile << "========================================" << std::endl;
    
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

Logger& Logger::getInstance(const std::string& filename) {
    static Logger instance(filename);
    return instance;
}

void Logger::log(LogLevel level, const std::string& message, const std::string& funcName) {
    std::lock_guard<std::mutex> guard(m_logMutex);
    m_logFile << "[" << currentDateTime() << "]" 
        << "[" << logLevelToString(level) << "]"
        <<"[" << funcName << "] " 
        << message << std::endl; 
}

std::string Logger::currentDateTime() {
    std::time_t now = std::time(nullptr);
    char buffer[80];

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", std::localtime(&now));
    return buffer;
}

std::string Logger::logLevelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}