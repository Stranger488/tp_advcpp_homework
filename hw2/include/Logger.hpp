#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <memory>
#include "BaseLogger.hpp"
#include "StdoutLogger.hpp"

namespace Log {

class Logger {
public:
    static Logger& get_instance();

    BaseLogger& get_global_logger();
    void set_global_logger(std::unique_ptr<BaseLogger> logger);

private:
    std::unique_ptr<BaseLogger> global_logger_;

    Logger();

    Logger(const Logger&) = delete;             // Copy construct
    Logger(Logger&&) = delete;                  // Move construct
    Logger& operator=(const Logger&) = delete;  // Copy assign
    Logger& operator=(Logger&&) = delete;       // Move assign
};

} // namespace Log


#endif // LOGGER_HPP
