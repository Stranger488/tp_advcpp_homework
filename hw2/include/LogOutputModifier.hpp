#ifndef LOGOUTPUTMODIFIER_HPP
#define LOGOUTPUTMODIFIER_HPP


#include <unordered_map>
#include "BaseLogger.hpp"

#define LOG_STRING_DEBUG "DEBUG"
#define LOG_STRING_INFO "INFO"
#define LOG_STRING_WARN "WARN"
#define LOG_STRING_ERROR "ERROR"

namespace Log {

enum LogColorCode {
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_DEFAULT = 39
};

const std::unordered_map<const Level, const LogColorCode> LEVEL_COLOR {
    { Level::DEBUG, LogColorCode::FG_DEFAULT },
    { Level::INFO, LogColorCode::FG_GREEN },
    { Level::WARN, LogColorCode::FG_YELLOW },
    { Level::ERROR, LogColorCode::FG_RED }
};

const std::unordered_map<const Level, const std::string> LEVEL_STRING {
    { Level::DEBUG, LOG_STRING_DEBUG },
    { Level::INFO, LOG_STRING_INFO },
    { Level::WARN, LOG_STRING_WARN },
    { Level::ERROR, LOG_STRING_ERROR }
};

class LogOutputModifier {
public:
    explicit LogOutputModifier(const Level lvl, bool is_colored);

    friend std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod);

private:
    LogColorCode code;
    std::string string;
    bool is_colored;
};

} // namespace Log


#endif // LOGOUTPUTMODIFIER_HPP
