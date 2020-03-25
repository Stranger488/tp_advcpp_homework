#ifndef STDOUTLOGGER_HPP
#define STDOUTLOGGER_HPP


class StdoutLogger : public BaseLogger {
public:
    StdoutLogger() = default;

    ~StdoutLogger() = default;

    void flush() override;

private:
    void log(const std::string &msg, const Level lvl) override;
};


#endif // STDOUTLOGGER_HPP
