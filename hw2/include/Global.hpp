#ifndef GLOBAL_HPP
#define GLOBAL_HPP

void create_file_logger();

void create_stdout_logger();

void create_stderr_logger()

void debug(const std::string &msg);

void info(const std::string &msg);

void warning(const std::string &msg);

void error(const std::string &msg);

#endif // GLOBAL_HPP
