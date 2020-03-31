#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>

#include "Socket.hpp"

namespace Tcp {

Socket::Socket() {
    int res = socket(AF_INET, SOCK_STREAM, 0);
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    fd_ = res;
}

Socket::~Socket() noexcept {
    close();
}

int Socket::get_fd() const {
    return fd_;
}

int Socket::extract_fd() {
    int new_fd = fd_;
    fd_ = -1;

    return new_fd;
}

void Socket::close() {
    ::close(fd_);
}

} // namespace Tcp
