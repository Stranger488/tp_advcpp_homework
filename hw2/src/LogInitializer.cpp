#include <getopt.h>
#include <iostream>
#include <Global.hpp>
#include "LogInitializer.hpp"

namespace Log {

LogInitializer::LogInitializer(int argc, char* argv[]) {
    lvl = Level::INFO;
    out = LogOutput::STDOUT;
    file = "log.log";

    parse_option(argc, argv);
    init_logger();
}

void LogInitializer::parse_option(int argc, char* argv[]) {
    const std::string short_opts("vqwfe", 5);

    const struct option long_opts[] = {
            { "verbose", no_argument, nullptr, 'v' },
            { "quiet", no_argument, nullptr, 'q' },
            { "warn", no_argument, nullptr, 'w' },
            { "log-to-stderr", no_argument, nullptr, 'e' },
            { "log-to-file", optional_argument, nullptr, 'f' },
            {nullptr, no_argument, nullptr, 0 }
    };

    int opt = -1;
    int option_index = -1;

    while(((opt = getopt_long(argc, argv, short_opts.c_str(), long_opts, &option_index)) != -1)) {
        if (opt == 'v') {
            lvl = Level::DEBUG;
        } else if (opt == 'q') {
            lvl = Level::ERROR;
        } else if (opt == 'w') {
            lvl = Level::WARN;
        } else if (opt == 'f') {
            out = LogOutput::FILE;
            if (optarg) {
                file = optarg;
            }
        } else if (opt == 'e') {
            out = Log::LogOutput::STDERR;
        } else {
            std::cerr << "Parse options error. Default level is INFO, default output is std::ostream." << std::endl;
        }
    }
}

void LogInitializer::init_logger() {
    if (out == LogOutput::FILE) {
        create_file_logger(file, lvl);
    } else if (out == LogOutput::STDOUT) {
        create_stdout_logger(lvl);
    } else if (out == LogOutput::STDERR) {
        create_stderr_logger(lvl);
    }
}

} // namespace Log