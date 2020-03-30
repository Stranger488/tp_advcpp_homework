#include <iostream>
#include <LogOutputModifier.hpp>
#include "StderrLogger.hpp"

namespace Log {

StderrLogger::StderrLogger(Level lvl) : BaseLogger(lvl) {}

void StderrLogger::flush() {
    std::cerr.flush();
}

void StderrLogger::log(const std::string& msg, Level lvl) {
    if (lvl >= level()) {
        std::cerr << LogOutputModifier(lvl, LogColorState::COLORED) << msg << std::endl;
    }
}

} // namespace Log
