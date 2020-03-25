#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP


#include <fstream>
#include "BaseLogger.hpp"

namespace Log {

class FileLogger : public BaseLogger {
public:
    FileLogger() = default;

    void flush() override;

private:
    std::ofstream file_out;

    void log(const std::string& msg, const Level lvl) override;
};

} // namespace Log

#endif // FILELOGGER_HPP
