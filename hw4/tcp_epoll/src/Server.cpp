#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>

#include "Server.hpp"

namespace Tcp_epoll {

void Server::create_epoll() {
    epoll_fd_ = epoll_create(1);
    if (epoll_fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}


void Server::add_epoll(int fd, uint32_t events) {
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}


void Server::modify_epoll(int fd, uint32_t events) {
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}


void Server::accept_clients() {
    sockaddr_in client_addr{};
    socklen_t addr_size = sizeof(client_addr);

    FileDescriptor fd;
    while (true) {
        fd = ::accept4(fd_,
                reinterpret_cast<sockaddr *>(&client_addr),
                &addr_size,
                SOCK_NONBLOCK);
        if (fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            }
            else {
                throw std::runtime_error(std::strerror(errno));
            }
        }

        add_epoll(fd, EPOLLIN | EPOLLOUT);
        fd.extract();
    }
}

void Server::handle_client(int fd, int event) {
    if (event & EPOLLHUP ||
        event & EPOLLERR) {
        ::close(fd_);
        return;
    }

    if (event & EPOLLIN) {
        //
    } else if (event & EPOLLOUT) {
        //
    }
}

} // namespace Tcp_epoll