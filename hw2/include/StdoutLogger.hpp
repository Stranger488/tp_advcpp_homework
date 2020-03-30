#ifndef STDOUTLOGGER_HPP
#define STDOUTLOGGER_HPP


#include "BaseLogger.hpp"

namespace Log {

class StdoutLogger : public BaseLogger {
public:
    StdoutLogger() = default;
    explicit StdoutLogger(Level lvl);

    void flush() override;

private:
    void log(const std::string& msg, Level lvl) override;
};

} // namespace Log


#endif // STDOUTLOGGER_HPP
