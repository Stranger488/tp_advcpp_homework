#include <iostream>
#include "StderrLogger.hpp"

namespace Log {

void StderrLogger::flush() {
    std::cerr << std::flush;
}

void StderrLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        std::cerr << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log