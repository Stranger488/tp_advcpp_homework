#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "pipe.hpp"

Pipe::Pipe() {
    if (pipe(fds_) < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

Pipe::~Pipe() noexcept {
    close_in();
    close_out();
}

int Pipe::get_pipe_in() {
    return fds_[0];
}

int Pipe::get_pipe_out() {
    return fds_[1];
}

int Pipe::extract_in() {
    int temp = fds_[0];
    fds_[0] = -1;
    return temp;
}

int Pipe::extract_out() {
    int temp = fds_[1];
    fds_[1] = -1;
    return temp;
}

void Pipe::close_in() {
    ::close(fds_[0]);
}

void Pipe::close_out() {
    ::close(fds_[1]);
}
