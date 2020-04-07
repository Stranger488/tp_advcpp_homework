#include "Logger.hpp"
#include "StdoutLogger.hpp"

namespace Log {

Logger::Logger() : global_logger_(std::make_unique<StdoutLogger>()) {}

Logger& Logger::get_instance() {
    static Logger logger;
    return logger;
}

BaseLogger& Logger::get_global_logger() {
    return *global_logger_;
}

void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
    global_logger_ = std::move(logger);
}

} // namespace Log

