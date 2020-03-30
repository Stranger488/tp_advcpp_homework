#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP


#include <fstream>
#include "BaseLogger.hpp"

namespace Log {

class FileLogger : public BaseLogger {
public:
    FileLogger();
    FileLogger(const std::string& file, Level lvl);

    void flush() override;

private:
    std::ofstream file_out_;

    void log(const std::string& msg, Level lvl) override;
};

} // namespace Log


#endif // FILELOGGER_HPP
