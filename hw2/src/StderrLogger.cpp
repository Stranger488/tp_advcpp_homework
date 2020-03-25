#include "StderrLogger.hpp"


void StderrLogger::flush() {
    std::cerr << std::flush;
}

void StderrLogger::log(const std::string &msg, const Level lvl) {
    return;
}
