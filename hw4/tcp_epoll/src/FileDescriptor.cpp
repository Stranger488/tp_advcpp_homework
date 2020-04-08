#include <unistd.h>
#include <utility>

#include "FileDescriptor.hpp"

namespace Tcp_epoll {

FileDescriptor::FileDescriptor(int fd) : fd_(fd) { }

FileDescriptor::FileDescriptor(FileDescriptor&& other) : fd_(other) {
    other.fd_ = -1;
}

FileDescriptor::~FileDescriptor() noexcept {
    close();
}

FileDescriptor& FileDescriptor::operator=(int fd) {
    close();
    fd_ = fd;

    return *this;
}

FileDescriptor::operator int() const {
    return fd_;
}

void FileDescriptor::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

int FileDescriptor::extract() {
    int temp_fd = fd_;
    fd_ = -1;

    return temp_fd;
}

} // namespace Tcp_epoll

