#include "util/logger.hpp"

int main() {
    
    try {
        Logger logger("./logs/log_test.log");
        logger.log(Logger::INFO, "Test Info Message");
        logger.log(Logger::WARNING, "Test Warning Message");
        logger.log(Logger::ERROR, "Test Error message");
    }
    catch (const std::exception& e) {
        std::cerr << "Logger error: " << e.what() << std::endl;
    }

    return 0;
}