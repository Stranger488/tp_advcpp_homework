#include <LogInitializer.hpp>
#include "Global.hpp"

int main(int argc, char* argv[]) {
    Log::LogInitializer log(argc, argv);

    Log::debug("debug msg");
    Log::info("info msg");
    Log::warning("warn msg");
    Log::error("error msg");

    return 0;
}
