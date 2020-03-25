#ifndef STDERRLOGGER_HPP
#define STDERRLOGGER_HPP


class StderrLogger : public BaseLogger {
public:
    StderrLogger() = default;

    ~StderrLogger() = default;

    void flush() override;

private:
    void log(const std::string &msg, const Level lvl) override;
};


#endif // STDERRLOGGER_HPP
