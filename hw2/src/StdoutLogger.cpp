#include <iostream>
#include "StdoutLogger.hpp"

namespace Log {

void StdoutLogger::flush() {
    std::cout << std::flush;
}

void StdoutLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        std::cout << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log