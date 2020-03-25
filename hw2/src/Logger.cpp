#include "Logger.hpp"


static Logger &get_instance() {
    static Logger logger;
    return logger;
}

void Logger::get_global_logger() {
    return;
}

void Logger::set_global_logger() {
    return;
}
