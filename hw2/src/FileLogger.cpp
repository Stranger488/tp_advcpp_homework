#include "FileLogger.hpp"
#include "LogOutputModifier.hpp"

namespace Log {

FileLogger::FileLogger() : file_out_("log.log") {
    if (!file_out_) {
        throw std::runtime_error("Error while creating file_.");
    }
}

FileLogger::FileLogger(const std::string& file, Level lvl)
    : file_out_(file), BaseLogger(lvl) {
    if (!file_out_) {
        throw std::runtime_error("Error while creating file_.");
    }
}

void FileLogger::flush() {
    file_out_.flush();
}

void FileLogger::log(const std::string& msg, Level lvl) {
    if (lvl >= level()) {
        file_out_ << LogOutputModifier(lvl, LogColorState::UNCOLORED) << msg << std::endl;
    }
}

} // namespace Log
