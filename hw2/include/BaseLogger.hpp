#ifndef BASELOGGER_HPP
#define BASELOGGER_HPP

enum class Level : unsigned {
    DEBUG, INFO, WARN, ERROR
};

class BaseLogger {
public:
    BaseLogger() = default;

    virtual ~BaseLogger() = default;

    void debug(const std::string &msg);

    void info(const std::string &msg);

    void warn(const std::string &msg);

    void error(const std::string &msg);

    void set_level(const Level lvl);

    Level level();

    virtual void flush() = 0;

private:
    Level level_ = Level::DEBUG;

    virtual void log(const std::string &msg, const Level lvl) = 0;
};


#endif // BASELOGGER_HPP
