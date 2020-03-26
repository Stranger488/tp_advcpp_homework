#include <Logger.hpp>
#include <FileLogger.hpp>
#include <StderrLogger.hpp>
#include <iostream>
#include "Global.hpp"

namespace Log {

void create_file_logger(const std::string& path, const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<FileLogger>(path, lvl));
}

void create_stdout_logger(const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<StdoutLogger>(lvl));
}

void create_stderr_logger(const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<StderrLogger>(lvl));
}

void debug(const std::string& msg) {
    std::unique_ptr<BaseLogger>& logger = Logger::get_instance().get_global_logger();
    if (logger) {
        logger->debug(msg);
    }
}

void info(const std::string& msg) {
    std::unique_ptr<BaseLogger>& logger = Logger::get_instance().get_global_logger();
    if (logger) {
        logger->info(msg);
    }
}

void warning(const std::string& msg) {
    std::unique_ptr<BaseLogger>& logger = Logger::get_instance().get_global_logger();
    if (logger) {
        logger->warn(msg);
    }
}

void error(const std::string& msg) {
    std::unique_ptr<BaseLogger>& logger = Logger::get_instance().get_global_logger();
    if (logger) {
        logger->error(msg);
    }
}

}