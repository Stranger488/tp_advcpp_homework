#ifndef BASELOGGER_HPP
#define BASELOGGER_HPP


#include <string>

namespace Log {

enum class Level {
    DEBUG, INFO, WARN, ERROR
};

class BaseLogger {
public:
    BaseLogger() = default;
    virtual ~BaseLogger() noexcept = default;

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

    void set_level(const Level lvl);
    const Level level();

    virtual void flush() = 0;

private:
    Level level_ = Level::DEBUG;

    virtual void log(const std::string& msg, const Level lvl) = 0;
};

} // namespace Log


#endif // BASELOGGER_HPP
