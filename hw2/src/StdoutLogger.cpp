#include "StdoutLogger.hpp"


void StdoutLogger::flush() {
    std::cout << std::flush;
}

void StdoutLogger::log(const std::string &msg, const Level lvl) {
    return;
}
