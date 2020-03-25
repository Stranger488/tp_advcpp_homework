#include "FileLogger.hpp"


void FileLogger::flush() {
    file_out << std::flush;
}

void FileLogger::log(const std::string &msg, const Level lvl) {
    return;
}
