#include <Logger.hpp>
#include <FileLogger.hpp>
#include <StderrLogger.hpp>

namespace Log {

std::unique_ptr<BaseLogger> create_file_logger(const std::string& path, Level lvl) {
    return std::make_unique<FileLogger>(path, lvl);
}

std::unique_ptr<BaseLogger> create_stdout_logger(Level lvl) {
    return std::make_unique<StdoutLogger>(lvl);
}

std::unique_ptr<BaseLogger> create_stderr_logger(Level lvl) {
    return std::make_unique<StderrLogger>(lvl);
}

void debug(const std::string& msg) {
    auto& logger = Logger::get_instance().get_global_logger();
    logger.debug(msg);
}

void info(const std::string& msg) {
    auto& logger = Logger::get_instance().get_global_logger();
    logger.info(msg);
}

void warning(const std::string& msg) {
    auto& logger = Logger::get_instance().get_global_logger();
    logger.warn(msg);
}

void error(const std::string& msg) {
    auto& logger = Logger::get_instance().get_global_logger();
    logger.error(msg);
}

} // namespace Log
