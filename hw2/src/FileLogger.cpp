#include "FileLogger.hpp"


namespace Log {

void FileLogger::flush() {
    file_out << std::flush;
}

void FileLogger::log(const std::string& msg, const Log::Level lvl) {
    if (lvl >= level()) {
        file_out << static_cast<int>(lvl) << " " << msg << std::endl;
    }
}

} // namespace Log