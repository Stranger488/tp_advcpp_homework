#include <iostream>
#include "StderrLogger.hpp"

namespace Log {

StderrLogger::StderrLogger(const Level lvl) : BaseLogger(lvl) {}

void StderrLogger::flush() {
    std::cerr.flush();
}

void StderrLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        std::cerr << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log