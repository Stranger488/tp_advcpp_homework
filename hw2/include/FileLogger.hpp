#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP


class FileLogger : public BaseLogger {
public:
    FileLogger() = default;

    ~FileLogger() = default;

private:
    std::ofstream file_out;

    void log(const std::string &msg, const Level lvl) override;
};


#endif // FILELOGGER_HPP
