#include <getopt.h>
#include <iostream>
#include <Global.hpp>
#include <Logger.hpp>
#include "LogInitializer.hpp"

LogInitializer::LogInitializer(int argc, char* argv[]) {
    lvl_ = Log::Level::INFO;
    out_ = LogOutput::STDOUT;
    file_ = "log.log";

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
            { nullptr, no_argument, nullptr, 0 }
    };

    int opt = -1;
    while(((opt = getopt_long(argc, argv, short_opts.c_str(), long_opts, nullptr)) != -1)) {
        if (opt == 'v') {
            lvl_ = Log::Level::DEBUG;
        } else if (opt == 'q') {
            lvl_ = Log::Level::ERROR;
        } else if (opt == 'w') {
            lvl_ = Log::Level::WARN;
        } else if (opt == 'f') {
            out_ = LogOutput::FILE;
            if (optarg) {
                file_ = optarg;
            }
        } else if (opt == 'e') {
            out_ = LogOutput::STDERR;
        } else {
            std::cerr << "Parse options error. Default level is INFO, default output is std::ostream." << std::endl;
        }
    }
}

void LogInitializer::init_logger() {
    Log::Logger& logger = Log::Logger::get_instance();

    if (out_ == LogOutput::FILE) {
        logger.set_global_logger(create_file_logger(file_, lvl_));
    } else if (out_ == LogOutput::STDOUT) {
        logger.set_global_logger(create_stdout_logger(lvl_));
    } else if (out_ == LogOutput::STDERR) {
        logger.set_global_logger(create_stderr_logger(lvl_));
    }
}
