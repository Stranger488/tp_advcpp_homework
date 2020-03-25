#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

namespace Log {

void create_file_logger();
void create_stdout_logger();
void create_stderr_logger();

void debug(const std::string& msg);
void info(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);

} // namespace Log


#endif // GLOBAL_HPP
