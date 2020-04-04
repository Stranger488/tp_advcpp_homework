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

LogOutputModifier::LogOutputModifier(Level lvl, LogState state)
    : code_(LEVEL_COLOR.at(lvl)), string_(LEVEL_STRING.at(lvl)), state_(state) {};

std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod) {
    if (mod.state_ == LogState::COLORED) {
        return os << "\033[" << static_cast<int>(mod.code_) << "m" << mod.string_ << ": ";
    } else if (mod.state_ == LogState::UNCOLORED) {
        return os << mod.string_ << ": ";
    } else if (mod.state_ == LogState::RESET) {
        return os << "\033[" << static_cast<int>(mod.code_) << "m";
    }

    return os;
}

const LogOutputModifier& LogOutputModifier::set_log_output(Level lvl, LogState state) {
    code_ = LEVEL_COLOR.at(lvl);
    string_ = LEVEL_STRING.at(lvl);
    state_ = state;

    return *this;
}

const LogOutputModifier& LogOutputModifier::reset_log_output() {
    code_ = LogColorCode::DEFAULT;
    string_ = '\0';
    state_ = LogState::RESET;

    return *this;
}

} // namespace Log
