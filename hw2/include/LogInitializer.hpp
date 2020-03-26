#ifndef LOGINITIALIZER_HPP
#define LOGINITIALIZER_HPP


#include "BaseLogger.hpp"

namespace Log {

enum class LogOutput {
    FILE, STDOUT, STDERR
};

class LogInitializer {
public:
    LogInitializer() = delete;
    explicit LogInitializer(int argc, char* argv[]);
    void parse_option(int argc, char* argv[]);
    void init_logger();

private:
    Level lvl;
    LogOutput out;
    std::string file;
};

} // namespace Log


#endif // LOGINITIALIZER_HPP
