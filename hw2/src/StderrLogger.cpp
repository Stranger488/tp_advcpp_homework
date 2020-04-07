#include <iostream>

#include "LogOutputModifier.hpp"
#include "StderrLogger.hpp"

namespace Log {

StderrLogger::StderrLogger(Level lvl) : BaseLogger(lvl) {}

void StderrLogger::flush() {
    std::cerr.flush();
}

void StderrLogger::log(const std::string& msg, Level lvl) {
    if (lvl >= level()) {
        LogOutputModifier output_modifier(lvl, LogState::COLORED);

        std::cerr << output_modifier << msg << std::endl;
        std::cerr << output_modifier.reset_log_output();
        flush();
    }
}

} // namespace Log
