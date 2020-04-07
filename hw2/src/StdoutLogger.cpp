#include <iostream>

#include "LogOutputModifier.hpp"
#include "StdoutLogger.hpp"

namespace Log {

StdoutLogger::StdoutLogger(const Level lvl) : BaseLogger(lvl) {}

void StdoutLogger::flush() {
    std::cout.flush();
}

void StdoutLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        LogOutputModifier output_modifier(lvl, LogState::COLORED);

        std::cout << output_modifier << msg << std::endl;
        std::cout << output_modifier.reset_log_output();
        flush();
    }
}

} // namespace Log
