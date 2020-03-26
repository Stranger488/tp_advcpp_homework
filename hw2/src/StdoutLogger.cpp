#include <iostream>
#include "StdoutLogger.hpp"

namespace Log {

StdoutLogger::StdoutLogger(const Level lvl) : BaseLogger(lvl) {}

void StdoutLogger::flush() {
    std::cout.flush();
}

void StdoutLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        std::cout << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log