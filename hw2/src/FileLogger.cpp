#include "FileLogger.hpp"


namespace Log {

FileLogger::FileLogger() : file_out("log.log") {
    if (!file_out) {
        throw std::runtime_error("Error while creating file.");
    }
}

FileLogger::FileLogger(const std::string& file, const Level lvl)
    : file_out(file), BaseLogger(lvl) {
    if (!file_out) {
        throw std::runtime_error("Error while creating file.");
    }
}

void FileLogger::flush() {
    file_out.flush();
}

void FileLogger::log(const std::string& msg, const Level lvl) {
    if (lvl >= level()) {
        file_out << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log