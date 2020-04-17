#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>

#include "Server.hpp"

namespace tcp_epoll {

void Server::init(const std::string& address, uint16_t port,
        Callback read_from_client_handler, Callback write_to_client_handler) {
    read_from_client_handler_ = std::move(read_from_client_handler);
    write_to_client_handler_ = std::move(write_to_client_handler);

    fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    open(address, port);
    create_epoll();
}

Server::~Server() noexcept {
    close();
}

void Server::open(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid IP address is given
        throw std::runtime_error(std::string("Error while converting IP address: invalid IP address."));
    } else if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    res = bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    set_max_connect(max_connect_);
    is_opened_ = true;
}

void Server::set_max_connect(size_t max_connect) {
    int res = listen(fd_, max_connect);
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void Server::create_epoll() {
    epoll_fd_ = epoll_create(1);
    if (epoll_fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    add_epoll(fd_, EPOLLIN);
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

    while (true) {
        int fd = ::accept4(fd_,
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

        std::string client_addr_string(addr_size, '\0');
        const char* res_addr = inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_string.data(), addr_size);
        if (res_addr == nullptr) {
            ::close(fd);
            throw std::runtime_error(std::strerror(errno));
        }

        client_addr_string.erase(client_addr_string.find('\0'));
        uint16_t client_addr_port = client_addr.sin_port;

        clients_.emplace(fd, Connection(client_addr_string, client_addr_port, fd));

        add_epoll(fd, EPOLLIN);
    }
}

void Server::handle_client(int fd, uint32_t event) {
    if (event & EPOLLHUP
        || event & EPOLLERR) {
        clients_.erase(fd);
        return;
    }

    if (event & EPOLLIN) {
        read_from_client_handler_(clients_.at(fd));
    } else if (event & EPOLLOUT) {
        write_to_client_handler_(clients_.at(fd));
    }
}

void Server::event_loop() {
    epoll_event events[EPOLL_SIZE];

    while (true) {
        int nfds = epoll_wait(epoll_fd_, events, EPOLL_SIZE, -1);
        if (nfds < 0) {
            if (errno == EINTR) {
                continue;
            }
            else {
                throw std::runtime_error(std::strerror(errno));
            }
        }

        for (int n = 0; n < nfds; n++) {
            int fd = events[n].data.fd;
            auto event = events[n].events;

            if (fd == fd_) {
                accept_clients();
            }
            else {
                clients_.at(fd).set_event(event);
                handle_client(fd, event);
            }
        }
    }
}

bool Server::is_opened() const {
    return is_opened_;
}

void Server::close() {
    ::close(fd_);

    is_opened_ = false;
}

} // namespace tcp_epoll
