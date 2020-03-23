#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "process.h"

Process::Process(const std::string &path) {
    int from_parent_to_child[2];
    int from_child_to_parent[2];

    if (pipe(from_parent_to_child) < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
    if (pipe(from_child_to_parent) < 0) {
        ::close(from_parent_to_child[0]);
        ::close(from_parent_to_child[1]);

        throw std::runtime_error(std::strerror(errno));
    }

    int parent_in = from_child_to_parent[0];
    int parent_out = from_parent_to_child[1];

    int child_in = from_parent_to_child[0];
    int child_out = from_child_to_parent[1];

    proc_pid = fork();
    if (proc_pid == -1) {
        ::close(parent_in);
        ::close(parent_out);

        ::close(child_in);
        ::close(child_out);

        throw std::runtime_error(std::strerror(errno));
    } else if (proc_pid == 0) {  // child
        ::close(parent_in);
        ::close(parent_out);

        if (dup2(child_in, STDIN_FILENO) < 0) {
            ::close(child_in);
            ::close(child_out);

            throw std::runtime_error(std::strerror(errno));
        }
        if (dup2(child_out, STDOUT_FILENO) < 0) {
            ::close(child_in);
            ::close(child_out);

            throw std::runtime_error(std::strerror(errno));
        }

        ::close(child_in);
        ::close(child_out);

        char *args[2] = {
                const_cast<char *>(path.c_str()),
                nullptr
        };

        if (execv(args[0], args) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    } else { // parent
        ::close(child_in);
        ::close(child_out);

        proc_in = parent_in;
        proc_out = parent_out;

        proc_in_state = ProcessStates::IS_OPENED;
        proc_out_state = ProcessStates::IS_OPENED;
    }
}

Process::~Process() {
    close();
}

size_t Process::write(const void *data, size_t len) {
    if (!isWritable()) {
        throw std::runtime_error("Process is not writable. 0 bytes were sent.");
    }

    ssize_t bytes = ::write(proc_out, data, len);

    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Process::writeExact(const void *data, size_t len) {
    if (!isWritable()) {
        throw std::runtime_error("Process is not writable. 0 bytes were sent.");
    }

    std::size_t sent = 0;
    std::size_t last_sent = 0;
    while (sent != len) {
        sent += write(static_cast<const char *>(data) + sent, len - sent);

        if (sent == last_sent) {
            throw std::runtime_error(std::to_string(sent) + " bytes were sent.");
        }

        last_sent = sent;
    }
}

size_t Process::read(void *data, size_t len) {
    if (!isReadable()) {
        throw std::runtime_error("Process is not readable. 0 bytes were received.");
    }

    ssize_t bytes = ::read(proc_in, data, len);
    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Process::readExact(void *data, size_t len) {
    if (!isReadable()) {
        throw std::runtime_error("Process is not readable. 0 bytes were received.");
    }

    std::size_t received = 0;
    std::size_t last_received = 0;
    while (received != len) {
        received += read(static_cast<char *>(data) + received, len - received);

        if (received == last_received) {
            throw std::runtime_error(std::to_string(received) + " bytes were received.");
        }

        last_received = received;
    }
}

bool Process::isReadable() const {
    return proc_in_state == ProcessStates::IS_OPENED;
}

bool Process::isWritable() const {
    return proc_out_state == ProcessStates::IS_OPENED;
}

void Process::closeStdin() {
    ::close(proc_in);
    proc_in_state = ProcessStates::IS_CLOSED;
}

void Process::close() {
    closeStdin();

    kill(proc_pid, SIGTERM);

    int status = 0;
    ::waitpid(proc_pid, &status, 0);
}

bool Process::isWorking() const {
    int status = 0;

    int res = waitpid(proc_pid, &status, WNOHANG);
    if (res == 0) { // child process is not finished
        return true;
    } else if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return false;
}
