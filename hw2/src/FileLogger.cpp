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
        LogOutputModifier output_modifier(lvl, LogState::UNCOLORED);

        file_out_ << output_modifier << msg << std::endl;
    }
}

} // namespace Log
