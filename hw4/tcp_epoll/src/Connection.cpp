#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <netinet/ip.h>
#include <sys/epoll.h>

#include "Connection.hpp"

namespace Tcp_epoll {

Connection::Connection(const std::string& address, uint16_t port)
        : dst_addr_(address), dst_port_(port), buf_(BUFSIZE, '\0') {
    fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    connect(dst_addr_, dst_port_);
}

Connection::Connection(const std::string& dst_addr, uint16_t dst_port,
                       FileDescriptor sock_fd)
        : dst_addr_(dst_addr), dst_port_(dst_port),
          fd_(std::move(sock_fd)), is_opened_(true), buf_(BUFSIZE, '\0') {}


Connection::Connection(Connection&& other)
        : fd_(std::move(other.fd_)),
        dst_addr_(std::move(other.dst_addr_)),
        dst_port_(other.dst_port_), is_opened_(other.is_opened_), buf_(std::move(other.buf_)) {}

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
    if (buf_.size() >= BUFSIZE) {
        buf_.clear();
        offset_read_ = 0;
        offset_write_ = 0;
    }

    ssize_t bytes = ::write(fd_, data, len);

    if (bytes < 0) {
        if (errno == EINTR) {
            return bytes;
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        else {
            return -1;
        }
    }

    return bytes;
}

ssize_t Connection::read(void* data, size_t len) {
    if (buf_.size() >= BUFSIZE) {
        buf_.clear();
        offset_read_ = 0;
        offset_write_ = 0;
        last_read_bytes_ = 0;
    }

    ssize_t bytes = ::read(fd_, data, len);

    if (bytes < 0) {
        if (errno == EINTR) {
            return bytes;
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        else {
            return -1;
        }
    }

    return bytes;
}

bool Connection::is_opened() const {
    return is_opened_;
}

void Connection::close() {
    fd_.close();

    is_opened_ = false;
}

size_t Connection::get_offset_read() const { return offset_read_; }
size_t Connection::get_offset_write() const { return offset_write_; }
size_t Connection::get_last_read_bytes() const { return last_read_bytes_; }

void Connection::set_event(uint32_t event) { event_ = event; }
void Connection::set_offset_read(size_t offset) { offset_read_ = offset; }
void Connection::set_offset_write(size_t offset) { offset_write_ = offset; }
void Connection::set_last_read_bytes(size_t bytes) { last_read_bytes_ = bytes; }

} // namespace Tcp_epoll
