#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <netinet/ip.h>
#include <iostream>
#include <sys/epoll.h>

#include "Connection.hpp"

namespace Tcp_epoll {

Connection::Connection(const std::string& address, uint16_t port)
        : dst_addr_(address), dst_port_(port) {
    fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    connect(dst_addr_, dst_port_);
}

Connection::Connection(const std::string& dst_addr, uint16_t dst_port,
                       FileDescriptor sock_fd)
        : dst_addr_(dst_addr), dst_port_(dst_port),
          fd_(std::move(sock_fd)), is_opened_(true) {}


Connection::Connection(Connection&& other)
        : fd_(other.fd_.extract())
        , dst_addr_(std::move(other.dst_addr_))
        , dst_port_(std::exchange(other.dst_port_, -1))
{
}

Connection::~Connection() noexcept {
    close();
}

void Connection::connect(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid IP address is given
        throw std::runtime_error(std::string("Error while converting IP address: invalid IP address."));
    } else if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    res = ::connect(fd_,
                    reinterpret_cast<sockaddr*>(&addr),
                    sizeof(addr));
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    is_opened_ = true;
}

ssize_t Connection::write(const void *data, size_t len) {
    ssize_t bytes = ::write(fd_, data, len);

    return bytes;
}

ssize_t Connection::read(void* data, size_t len) {
    ssize_t bytes = ::read(fd_, data, len);

    return bytes;
}

bool Connection::is_opened() const {
    return is_opened_;
}

void Connection::close() {
    fd_.close();

    is_opened_ = false;
}

} // namespace Tcp_epoll
