#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <getopt.h>
#include "BaseLogger.hpp"

namespace Log {

void create_file_logger(const std::string& path, const Level lvl);
void create_stdout_logger(const Level lvl);
void create_stderr_logger(const Level lvl);

void debug(const std::string& msg);
void info(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);

} // namespace Log


#endif // GLOBAL_HPP
