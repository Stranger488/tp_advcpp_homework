#include "BaseLogger.hpp"


void BaseLogger::debug(const std::string &msg) {
    log(msg, Level::DEBUG);
}

void BaseLogger::info(const std::string &msg) {
    log(msg, Level::INFO);
}

void BaseLogger::warn(const std::string &msg) {
    log(msg, Level::WARN);
}

void BaseLogger::ERROR(const std::string &msg) {
    log(msg, Level::ERROR);
}


void BaseLogger::set_level(const Level lvl) {
    level_ = lvl;
}

const Level BaseLogger::level() {
    return level_;
}
