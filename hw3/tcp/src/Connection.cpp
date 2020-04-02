#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <netinet/ip.h>

#include "Connection.hpp"

namespace Tcp {

Connection::Connection(const std::string& address, uint16_t port)
    : dst_addr_(address), dst_port_(port) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    connect(dst_addr_, dst_port_);
}

Connection::Connection(const std::string& dst_addr, uint16_t dst_port,
                       int sock_fd)
                       : dst_addr_(dst_addr), dst_port_(dst_port),
                       fd_(sock_fd), is_opened_(true) {}

Connection::~Connection() noexcept {
    close();
}

void Connection::connect(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid IP address is given
        close();
        throw std::runtime_error(std::string("Error while converting IP address: invalid IP address."));
    } else if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    res = ::connect(fd_,
                    reinterpret_cast<sockaddr*>(&addr),
                    sizeof(addr));
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    is_opened_ = true;
}

void Connection::set_timeout(time_t time, SocketTimeOutType timeout_type) {
    timeval timeout{ .tv_sec = time, .tv_usec= 0 };

    int res = setsockopt(fd_,
                         SOL_SOCKET,
                         static_cast<int>(timeout_type),
                         &timeout,
                         sizeof(timeout));
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

size_t Connection::write(const void *data, size_t len) {
    ssize_t bytes = ::write(fd_, data, len);
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
    ssize_t bytes = ::read(fd_, data, len);
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
    ::close(fd_);

    is_opened_ = false;
}

} // namespace Tcp