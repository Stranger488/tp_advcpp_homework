#include <cstring>
#include <stdexcept>
#include <unistd.h>

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

    int proc_pid = fork();
    switch (proc_pid) {
        case -1:
            throw std::runtime_error(std::strerror(errno));
        case 0: {  // child
            ::close(parent_in);
            ::close(parent_out);

            if (dup2(child_in, STDIN_FILENO) < 0 ||
                dup2(child_out, STDOUT_FILENO) < 0) {
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

            exit(EXIT_SUCCESS);
        }
        default: { // parent
            ::close(child_in);
            ::close(child_out);

            proc_in = parent_in;
            proc_out = parent_out;

            proc_in_state = IS_CLOSED;

            break;
        }
    }
}

Process::~Process() {

}

size_t Process::write(const void *data, size_t len) {
    ssize_t bytes = ::write(proc_out, data, len);

    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Process::writeExact(const void *data, size_t len) {

}

size_t Process::read(void *data, size_t len) {
    ssize_t bytes = ::read(proc_in, data, len);

    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Process::readExact(void *data, size_t len) {

}

bool Process::isReadable() const {
    return proc_in_state;
}

bool Process::isWritable() const {
    return proc_out_state;
}

void Process::closeStdin() {
    ::close(proc_in);
    proc_in_state = IS_CLOSED;
}

void Process::closeStdout() {
    ::close(proc_out);
    proc_out_state = IS_CLOSED;
}

void Process::close() {
    closeStdin();
    closeStdout();
}
