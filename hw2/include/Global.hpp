#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <memory>

#include "BaseLogger.hpp"

namespace Log {

std::unique_ptr<BaseLogger> create_file_logger(const std::string& path, Level lvl);
std::unique_ptr<BaseLogger> create_stdout_logger(Level lvl);
std::unique_ptr<BaseLogger> create_stderr_logger(Level lvl);

void debug(const std::string& msg);
void info(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);

} // namespace Log


#endif // GLOBAL_HPP
