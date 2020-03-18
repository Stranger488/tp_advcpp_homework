#ifndef HOMEWORK_PROCESS_H
#define HOMEWORK_PROCESS_H

#include <string>

class Process {
public:
    explicit Process(const std::string &path);

    ~Process();

    size_t write(const void *data, size_t len);

    void writeExact(const void *data, size_t len);

    size_t read(void *data, size_t len);

    void readExact(void *data, size_t len);

    bool isReadable() const;

    bool isWritable() const;

    void closeStdin();

    void closeStdout();

    void close();

private:
    enum ProcessStates {
        IS_CLOSED,
        IS_OPENED
    };

    int proc_in;
    int proc_out;

    ProcessStates proc_in_state;
    ProcessStates proc_out_state;

    pid_t proc_pid;
};

#endif //HOMEWORK_PROCESS_H
