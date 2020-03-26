#include "BaseLogger.hpp"
#include "Global.hpp"

int main(int argc, char* argv[]) {
    Log::create_stderr_logger(Log::Level::INFO);

    Log::debug("debug msg");
    Log::info("info msg");
    Log::warning("warn msg");
    Log::error("error msg");

    return 0;
}