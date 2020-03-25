#ifndef LOGGER_HPP
#define LOGGER_HPP


class Logger {
public:
    static Logger &get_instance();

private:
    Logger(Logger const &) = delete;             // Copy construct
    Logger(Logger &&) = delete;                  // Move construct
    Logger &operator=(Logger const &) = delete;  // Copy assign
    Logger &operator=(Logger &&) = delete;      // Move assign

    void get_global_logger();

    void set_global_logger();
};


#endif // LOGGER_HPP
