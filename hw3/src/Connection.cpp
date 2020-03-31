#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <netinet/ip.h>
#include <arpa/inet.h>

#include "Connection.hpp"

namespace Tcp {

Connection::Connection(const std::string& address, uint16_t port)
    : dst_addr_(address), dst_port_(port), fd_(Socket()) {
    connect(dst_addr_, dst_port_);
}

Connection::~Connection() noexcept {
    close();
}

void Connection::connect(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid ip address is given
        throw std::runtime_error(std::string("Error while converting ip address: invalid ip address."));
    } else if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    res = ::connect(fd_.get_fd(),
            reinterpret_cast<sockaddr*>(&addr),
            sizeof(addr));
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void Connection::set_timeout(time_t time) {
    timeval timeout{.tv_sec = 5, .tv_usec= 0};
    int res = setsockopt(fd_.get_fd(),
                         SOL_SOCKET,
                         SO_SNDTIMEO,
                         &timeout,
                         sizeof(timeout));
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

size_t Connection::write(const void *data, size_t len) {
    ssize_t bytes = ::write(fd_.get_fd(), data, len);
    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Connection::writeExact(const void *data, size_t len) {
    std::size_t written = 0;

    while (written != len) {
        written += write(static_cast<const char *>(data) + written, len - written);
    }
}

size_t Connection::read(void *data, size_t len) {
    ssize_t bytes = ::read(fd_.get_fd(), data, len);
    if (bytes < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    return static_cast<size_t>(bytes);
}

void Connection::readExact(void *data, size_t len) {
    std::size_t received = 0;
    while (received != len) {
        received += read(static_cast<char *>(data) + received, len - received);
    }
}

bool Connection::is_opened() const {
    return is_opened_;
}

void Connection::close() {
    fd_.close();

    is_opened_ = false;
}

} // namespace Tcp