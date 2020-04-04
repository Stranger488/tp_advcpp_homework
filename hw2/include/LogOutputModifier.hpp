#ifndef LOGOUTPUTMODIFIER_HPP
#define LOGOUTPUTMODIFIER_HPP


#include "BaseLogger.hpp"

namespace Log {

enum class LogColorCode {
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    DEFAULT = 39
};

enum class LogState {
    COLORED, UNCOLORED, RESET
};

class LogOutputModifier {
public:
    LogOutputModifier(Level lvl, LogState state);

    friend std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod);

    const LogOutputModifier& set_log_output(Level lvl, LogState state);
    const LogOutputModifier& reset_log_output();

private:
    LogColorCode code_;
    std::string string_;
    LogState state_;
};

} // namespace Log


#endif // LOGOUTPUTMODIFIER_HPP
