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
    explicit BaseLogger(Level lvl);
    virtual ~BaseLogger() noexcept = default;

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

    void set_level(Level lvl);
    Level level() const;

    virtual void flush() = 0;

private:
    Level level_ = Level::DEBUG;

    virtual void log(const std::string& msg, Level lvl) = 0;
};

} // namespace Log


#endif // BASELOGGER_HPP
