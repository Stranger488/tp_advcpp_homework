#ifndef LOGOUTPUTMODIFIER_HPP
#define LOGOUTPUTMODIFIER_HPP


#include <unordered_map>
#include "BaseLogger.hpp"

namespace Log {

enum class LogColorCode {
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    DEFAULT = 39
};

enum class LogColorState {
    COLORED, UNCOLORED
};

class LogOutputModifier {
public:
    LogOutputModifier(Level lvl, LogColorState color_state);

    friend std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod);

private:
    LogColorCode code_;
    std::string string_;
    LogColorState state_;
};

} // namespace Log


#endif // LOGOUTPUTMODIFIER_HPP
