#include "LogOutputModifier.hpp"
#include <ostream>

namespace Log {

LogOutputModifier::LogOutputModifier(const Level lvl, bool is_colored)
    : code(LEVEL_COLOR.at(lvl)), string(LEVEL_STRING.at(lvl)), is_colored(is_colored) {};

std::ostream& operator<<(std::ostream& os, const LogOutputModifier& mod) {
    if (!mod.is_colored) {
        return os << "\033[" << mod.code << "m" << mod.string << ": ";
    }

    return os << mod.string << ": ";
}

} // namespace Log