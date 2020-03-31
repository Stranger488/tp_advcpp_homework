#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>
#include <iostream>

#include "Server.hpp"

namespace Tcp {

Server::Server(const std::string& address, uint16_t port) {
    int res = socket(AF_INET, SOCK_STREAM, 0);
    if (res < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
    fd_ = res;

    open(address, port);
}

Server::~Server() noexcept {
    close();
}

void Server::open(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid ip address is given
        close();
        throw std::runtime_error(std::string("Error while converting ip address: invalid ip address."));
    } else if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    res = bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    set_max_connect(max_connect_);
}

void Server::set_max_connect(int max_connect) {
    int res = listen(fd_, max_connect);
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }
}

Connection Server::accept() {
    sockaddr_in client_addr{};
    socklen_t addr_size = sizeof(client_addr);
    int res = ::accept(fd_,
                     reinterpret_cast<sockaddr*>(&client_addr),
                     &addr_size);
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    std::string buf(addr_size, '\0');
    const char* temp = inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf.data(), addr_size);
    if (temp == nullptr) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    std::cout << buf << std::endl;

    return Connection(buf, client_addr.sin_port);
}

bool Server::is_opened() const {
    return is_opened_;
}

void Server::close() {
    ::close(fd_);

    is_opened_ = false;
}

} // namespace Tcp
