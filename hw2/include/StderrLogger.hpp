#ifndef STDERRLOGGER_HPP
#define STDERRLOGGER_HPP


#include "BaseLogger.hpp"

namespace Log {

class StderrLogger : public BaseLogger {
public:
    StderrLogger() = default;
    StderrLogger(const Level lvl);

    void flush() override;

private:
    void log(const std::string& msg, const Level lvl) override;
};

} // namespace Log


#endif // STDERRLOGGER_HPP
