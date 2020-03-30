#include "LogOutputModifier.hpp"
#include <ostream>

namespace Log {

static const std::unordered_map<Level, LogColorCode> LEVEL_COLOR {
        { Level::DEBUG, LogColorCode::DEFAULT },
        { Level::INFO, LogColorCode::GREEN },
        { Level::WARN, LogColorCode::YELLOW },
        { Level::ERROR, LogColorCode::RED }
};

static const std::unordered_map<Level, const std::string> LEVEL_STRING {
        { Level::DEBUG, "DEBUG" },
        { Level::INFO, "INFO" },
        { Level::WARN, "WARN" },
        { Level::ERROR, "ERROR" }
};

LogOutputModifier::LogOutputModifier(Level lvl, LogColorState color_state)
    : code_(LEVEL_COLOR.at(lvl)), string_(LEVEL_STRING.at(lvl)), state_(color_state) {};

std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod) {
    if (mod.state_ == LogColorState::COLORED) {
        return os << "\033[" << static_cast<int>(mod.code_) << "m" << mod.string_ << ": ";
    }

    return os << mod.string_ << ": ";
}

} // namespace Log
