#include <iostream>
#include <LogOutputModifier.hpp>
#include "StderrLogger.hpp"

namespace Log {

StderrLogger::StderrLogger(const Level lvl) : BaseLogger(lvl) {}

void StderrLogger::flush() {
    std::cerr.flush();
}

void StderrLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        std::cerr << LogOutputModifier(lvl ,false) << msg << std::endl;
    }
}

} // namespace Log
