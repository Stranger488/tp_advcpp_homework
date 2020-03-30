#ifndef LOGINITIALIZER_HPP
#define LOGINITIALIZER_HPP


#include "BaseLogger.hpp"

enum class LogOutput {
    FILE, STDOUT, STDERR
};

class LogInitializer {
public:
    LogInitializer() = delete;
    LogInitializer(int argc, char* argv[]);
    void parse_option(int argc, char* argv[]);
    void init_logger();

private:
    Log::Level lvl_;
    LogOutput out_;
    std::string file_;
};


#endif // LOGINITIALIZER_HPP
